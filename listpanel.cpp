#include "listpanel.h"
#include "ui_listpanel.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QTime>
#include <QInputDialog>
#include "playlist_item.h"
#include "commentlist_item.h"
#include "datamanager.h"
#include "commentdialog.h"

void ListPanel::loadVideos(const QString& folderPath, QListWidget* listWidget) {
    QDir dir(folderPath);
    QStringList filters;
#if defined(_WIN32)
    filters << "*.wmv";
#else
    filters << "*.mp4" << "*.mov";
#endif
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);

    listWidget->clear(); // 清空列表

    for (const QString& fileName : dir.entryList()) {
        QString filePath = dir.absoluteFilePath(fileName);

        // 设置标题和默认时长
        QString title = QFileInfo(fileName).fileName(); // 使用文件名作为标题

        // 加载缩略图
        QString thumbPath = filePath.left(filePath.lastIndexOf('.')) + ".png";
        QPixmap thumbnail;
        if (QFile::exists(thumbPath)) {
            thumbnail.load(thumbPath);
        } else {
            thumbnail = QPixmap(":/images/default_thumbnail.png"); // 使用默认缩略图
        }

        // 加载视频时长
        QString duration = "--:--"; // 默认时长
        QMediaPlayer mediaPlayer;
        mediaPlayer.setMedia(QUrl::fromLocalFile(filePath));
        QEventLoop loop;
        QObject::connect(&mediaPlayer, &QMediaPlayer::durationChanged, [&mediaPlayer, &duration, &loop](qint64 mediaDuration) {
            duration = QTime(0, 0).addMSecs(mediaDuration).toString("mm:ss");
            loop.quit();
        });

        // 等待元数据加载完成
        mediaPlayer.play();
        loop.exec();

        // 创建自定义列表项
        QListWidgetItem* item = new QListWidgetItem(listWidget);
        PlayListItem* customItem = new PlayListItem;
        customItem->updateInfo(title, thumbnail, duration, filePath); // 更新时传入视频路径

        item->setSizeHint(customItem->sizeHint());
        listWidget->setItemWidget(item, customItem);
    }
}

// 在路径中插入零宽度空格，强制换行
QString ListPanel::insertZeroWidthSpace(const QString& text) {
    QString result;
    for (int i = 0; i < text.length(); ++i) {
        result.append(text[i]);
        result.append("\u200B"); // 插入零宽度空格
    }
    return result;
}

void ListPanel::openFolder(const QString& folderPath) {
    if (folderPath.isEmpty() || !QDir(folderPath).exists()) {
        QMessageBox::warning(this, "Invalid Path", "The specified folder does not exist.");
        return;
    }

    // 加载视频
    loadVideos(folderPath, ui->PlaylistListWidget);

    // 更新路径显示
    ui->CurrentPathLabel->setText(insertZeroWidthSpace(folderPath));
    ui->CurrentPathLabel->setToolTip(folderPath);

    // 初始化或更新 DataManager
    if (dataManager) {
        delete dataManager;
    }
    dataManager = new DataManager(folderPath);

    // 禁用按钮，因为还没有选择具体文件
    updateButtonStates(false);
}

// 批量设置点赞/收藏/新建评论的按钮状态
void ListPanel::updateButtonStates(bool enabled) {
    ui->FavouriteButton->setEnabled(enabled);
    ui->FavouriteOnButton->setEnabled(enabled);
    ui->CollectButton->setEnabled(enabled);
    ui->CollectOnButton->setEnabled(enabled);
    ui->NewCommentButton->setEnabled(enabled);
}

ListPanel::ListPanel(QWidget *parent, QHBoxLayout *top, ThePlayer *player, ControlPanel *controlPanel) :
    QWidget(parent),
    ui(new Ui::ListPanel),
    topLayout(top),
    player(player),
    controlPanel(controlPanel),
    currentVideoPath(""),
    dataManager(nullptr)
{
    ui->setupUi(this);

    this->setMaximumWidth(400);
    ui->FavouriteOnButton->hide();
    ui->CollectOnButton->hide();
    ui->CommentOnButton->hide();
    ui->CommentWidget->hide();
    ui->PlaylistButton->hide();

    // 初始化按钮为禁用状态
    updateButtonStates(false);

    // 设置Now Playing为空
    PlayListItem* blankListItem = new PlayListItem();
    ui->PlayingItemWidget->layout()->addWidget(blankListItem);
    blankListItem->updateInfo("No video playing.", QPixmap(":/images/default_thumbnail.png"), 0, "No Path"); // 更新时传入视频路径

    // 点击打开文件夹按钮时
    QObject::connect(ui->OpenFolderButton, &QPushButton::clicked, [this]() {
        QString folderPath = QFileDialog::getExistingDirectory(this, "Select Video Folder");
        openFolder(folderPath);
    });

    // 选择列表项，播放对应的视频，并加载用户数据
    QObject::connect(ui->PlaylistListWidget, &QListWidget::itemSelectionChanged, this, &ListPanel::playSelectedVideo);


    // 点赞和收藏按钮
    QObject::connect(ui->FavouriteButton, &QPushButton::clicked, this, &ListPanel::onLikeClicked);
    QObject::connect(ui->FavouriteOnButton, &QPushButton::clicked, this, &ListPanel::onLikeClicked);
    QObject::connect(ui->CollectButton, &QPushButton::clicked, this, &ListPanel::onCollectClicked);
    QObject::connect(ui->CollectOnButton, &QPushButton::clicked, this, &ListPanel::onCollectClicked);

    // 新增评论
    QObject::connect(ui->NewCommentButton, &QPushButton::clicked, this, &ListPanel::onNewComment);

    // 切换列表
    QObject::connect(ui->CommentButton, &QPushButton::clicked, [&]() {
        ui->CommentButton->hide();
        ui->CommentOnButton->show();
        ui->PlaylistButton->show();
        ui->PlaylistOnButton->hide();
        ui->CommentWidget->show();
        ui->PlaylistWidget->hide();
    });
    QObject::connect(ui->PlaylistButton, &QPushButton::clicked, [&]() {
        ui->CommentButton->show();
        ui->CommentOnButton->hide();
        ui->PlaylistButton->hide();
        ui->PlaylistOnButton->show();
        ui->CommentWidget->hide();
        ui->PlaylistWidget->show();
    });

    // 关闭listPanel
    QObject::connect(ui->CloseButton, &QPushButton::clicked, [&]() {
        controlPanel->PlaylistButton->show();
        controlPanel->PlaylistOpenButton->hide();
        this->hide();
    });

    // 播放模式
    currentPlayMode = PlayMode::ListLoop;
    QObject::connect(ui->PlayModeListLoopButton, &QPushButton::clicked, this, &ListPanel::onPlayModeButtonClicked);
    QObject::connect(ui->PlayModeSingleLoopButton, &QPushButton::clicked, this, &ListPanel::onPlayModeButtonClicked);
    QObject::connect(ui->PlayModeRandomLoopButton, &QPushButton::clicked, this, &ListPanel::onPlayModeButtonClicked);
    QObject::connect(ui->PlayModeNoLoopButton, &QPushButton::clicked, this, &ListPanel::onPlayModeButtonClicked);
    // 连接播放器播放结束信号和listpanel的下一步行为
    connect(player, &ThePlayer::playbackFinished, this, &ListPanel::handlePlaybackFinished);
    // 初始化播放模式ui
    updatePlayModeUI();

    // 绑定control中的SkipNext和Prev按钮
    QObject::connect(controlPanel->SkipPrevButton, &QPushButton::clicked, this, &ListPanel::playPreviousVideo);
    QObject::connect(controlPanel->SkipNextButton, &QPushButton::clicked, this, &ListPanel::playNextVideo);
}

void ListPanel::playSelectedVideo() {
    if (player) {
        player->stopManually(); // 确保播放器进入手动停止模式
    }

    QListWidgetItem* item = ui->PlaylistListWidget->currentItem();
    if (!item) return;

    PlayListItem* customItem = qobject_cast<PlayListItem*>(ui->PlaylistListWidget->itemWidget(item));
    if (!customItem || !player) return;

    currentVideoPath = customItem->getVideoPath();
    player->playVideo(currentVideoPath);

    // 更新点赞和收藏状态
    updateLikeButton(dataManager->getLikeStatus(currentVideoPath));
    updateCollectButton(dataManager->getCollectStatus(currentVideoPath));

    // 刷新评论列表
    refreshComments();

    // 启用按钮
    updateButtonStates(true);

    // 更新“正在播放”区域
    if (ui->PlayingItemWidget) {
        // 清除 PlayingItemWidget 中的内容
        while (QLayoutItem* child = ui->PlayingItemWidget->layout()->takeAt(0)) {
            if (child->widget()) {
                delete child->widget();
            }
            delete child;
        }

        // 创建一个新的 Widget，复制 customItem 的信息
        PlayListItem* playingItem = new PlayListItem;
        playingItem->updateInfo(
            customItem->getTitle(),
            customItem->getThumbnail(),
            customItem->getDuration(),
            customItem->getVideoPath()
        );

        // 将复制的自定义列表项添加到 PlayingItemWidget
        ui->PlayingItemWidget->layout()->addWidget(playingItem);
    }
}

void ListPanel::refreshComments() {
    ui->CommentListWidget->clear();

    // 视频路径为空的时候就禁用按钮
    if (currentVideoPath.isEmpty()) {
        updateButtonStates(false);
        return;
    }

    QList<QVariantMap> comments = dataManager->getComments(currentVideoPath);
    for (const QVariantMap &commentData : comments) {
        int id = commentData["id"].toInt();
        QString username = commentData["username"].toString();
        QString content = commentData["content"].toString();

        CommentListItem *commentItem = new CommentListItem(id, username, content, this);

        QObject::connect(commentItem, &CommentListItem::editRequested, this, &ListPanel::onEditCommentRequested);
        connect(commentItem, &CommentListItem::deleteRequested, this, &ListPanel::onDeleteComment);

        QListWidgetItem *listItem = new QListWidgetItem(ui->CommentListWidget);
        listItem->setSizeHint(commentItem->sizeHint());
        ui->CommentListWidget->setItemWidget(listItem, commentItem);
    }
}

void ListPanel::onNewComment() {
    // 记录当前播放状态并暂停
    bool wasPlaying = (player->state() == QMediaPlayer::PlayingState);
    player->pause();

    // 创建评论对话框
    CommentDialog *dialog = new CommentDialog(this);
    dialog->setDialogMode(false); // 新建模式
    dialog->setWindowModality(Qt::ApplicationModal);

    // 显示对话框并处理结果
    if (dialog->exec() == QDialog::Accepted) {
        QString username = dialog->getUsername();
        QString content = dialog->getCommentContent();

        if (!username.isEmpty() && !content.isEmpty()) {
            dataManager->addComment(currentVideoPath, username, content);
            refreshComments();
        }
    }

    // 恢复播放状态
    if (wasPlaying) {
        player->play();
    }

    delete dialog;
}


// 处理编辑请求
void ListPanel::onEditCommentRequested(int commentId, const QString &username, const QString &content) {
    // 记录当前播放状态并暂停
    bool wasPlaying = (player->state() == QMediaPlayer::PlayingState);
    player->pause();

    // 创建评论对话框
    CommentDialog *dialog = new CommentDialog(this);
    dialog->setDialogMode(true); // 编辑模式
    dialog->setCommentData(username, content);
    dialog->setWindowModality(Qt::ApplicationModal);

    // 显示对话框并处理结果
    if (dialog->exec() == QDialog::Accepted) {
        QString newUsername = dialog->getUsername();
        QString newContent = dialog->getCommentContent();

        if (!newUsername.isEmpty() && !newContent.isEmpty()) {
            dataManager->editComment(currentVideoPath, commentId, newUsername, newContent);
            refreshComments();
        }
    }

    // 恢复播放状态
    if (wasPlaying) {
        player->play();
    }

    delete dialog;
}

void ListPanel::onDeleteComment(int commentId) {
    dataManager->deleteComment(currentVideoPath, commentId);
    refreshComments();
}

void ListPanel::onLikeClicked() {
    bool currentStatus = dataManager->getLikeStatus(currentVideoPath);
    dataManager->setLikeStatus(currentVideoPath, !currentStatus);
    updateLikeButton(!currentStatus);
}

void ListPanel::updateLikeButton(bool liked) {
    ui->FavouriteButton->setVisible(!liked);
    ui->FavouriteOnButton->setVisible(liked);
}

void ListPanel::onCollectClicked() {
    bool currentStatus = dataManager->getCollectStatus(currentVideoPath);
    dataManager->setCollectStatus(currentVideoPath, !currentStatus);
    updateCollectButton(!currentStatus);
}

void ListPanel::updateCollectButton(bool collected) {
    ui->CollectButton->setVisible(!collected);
    ui->CollectOnButton->setVisible(collected);
}

// 播放模式相关
void ListPanel::onPlayModeButtonClicked() {
    // 切换到下一种播放模式
    switch (currentPlayMode) {
        case PlayMode::ListLoop:
            currentPlayMode = PlayMode::SingleLoop;
            break;
        case PlayMode::SingleLoop:
            currentPlayMode = PlayMode::RandomLoop;
            break;
        case PlayMode::RandomLoop:
            currentPlayMode = PlayMode::NoLoop;
            break;
        case PlayMode::NoLoop:
            currentPlayMode = PlayMode::ListLoop;
            break;
    }

    // 更新界面
    updatePlayModeUI();
}

void ListPanel::updatePlayModeUI() {
    // 更新按钮状态
    ui->PlayModeListLoopButton->setVisible(currentPlayMode == PlayMode::ListLoop);
    ui->PlayModeSingleLoopButton->setVisible(currentPlayMode == PlayMode::SingleLoop);
    ui->PlayModeRandomLoopButton->setVisible(currentPlayMode == PlayMode::RandomLoop);
    ui->PlayModeNoLoopButton->setVisible(currentPlayMode == PlayMode::NoLoop);

    // 随机模式下禁用上一个按钮
    controlPanel->SkipPrevButton->setDisabled(currentPlayMode == PlayMode::RandomLoop);

    // 更新播放模式标签
    switch (currentPlayMode) {
        case PlayMode::ListLoop:
            ui->PlayModeLabel->setText("Play Mode: List Loop");
            break;
        case PlayMode::SingleLoop:
            ui->PlayModeLabel->setText("Play Mode: Single Loop");
            break;
        case PlayMode::RandomLoop:
            ui->PlayModeLabel->setText("Play Mode: Random Loop");
            break;
        case PlayMode::NoLoop:
            ui->PlayModeLabel->setText("Play Mode: No Loop");
            break;
    }
}

void ListPanel::handlePlaybackFinished() {
    if (currentPlayMode == PlayMode::NoLoop) {
        // 播放完成后不进行任何操作
        return;
    }

    int currentIndex = ui->PlaylistListWidget->currentRow();
    int totalItems = ui->PlaylistListWidget->count();

    // 根据播放模式选择下一个播放的项目
    if (currentPlayMode == PlayMode::ListLoop) {
        currentIndex = (currentIndex + 1) % totalItems;
    } else if (currentPlayMode == PlayMode::SingleLoop) {
        // 保持 currentIndex 不变
    } else if (currentPlayMode == PlayMode::RandomLoop) {
        int newIndex;
        do {
            newIndex = qrand() % totalItems; // 随机选择下一个
        } while (newIndex == currentIndex && totalItems > 1); // 确保新索引不同于当前索引
        currentIndex = newIndex;
    }

    // 设置当前选择项
    ui->PlaylistListWidget->setCurrentRow(currentIndex);

    // 调用封装好的播放逻辑
     playSelectedVideo();
}

void ListPanel::playNextVideo() {
    int currentIndex = ui->PlaylistListWidget->currentRow();
    int totalItems = ui->PlaylistListWidget->count();

    if (currentPlayMode == PlayMode::RandomLoop) {
        int nextIndex = qrand() % totalItems;
        while (nextIndex == currentIndex) {
            nextIndex = qrand() % totalItems; // 确保随机到不同的视频
        }
        currentIndex = nextIndex;
    } else {
        currentIndex = (currentIndex + 1) % totalItems;
    }

    ui->PlaylistListWidget->setCurrentRow(currentIndex);
    playSelectedVideo();
}

void ListPanel::playPreviousVideo() {
    if (currentPlayMode == PlayMode::RandomLoop) {
        return; // 随机模式下禁用“上一个”按钮
    }

    int currentIndex = ui->PlaylistListWidget->currentRow();
    int totalItems = ui->PlaylistListWidget->count();
    currentIndex = (currentIndex - 1 + totalItems) % totalItems;

    ui->PlaylistListWidget->setCurrentRow(currentIndex);
    playSelectedVideo();
}

ListPanel::~ListPanel() {
    delete ui;
    if (dataManager) {
        delete dataManager;
    }
}
