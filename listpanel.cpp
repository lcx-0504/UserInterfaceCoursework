//#include "listpanel.h"
//#include "ui_listpanel.h"
//#include <QMessageBox>
//#include <QFileDialog>
//#include <QMediaPlayer>
//#include <QTime>
//#include "playlist_item.h"
//void ListPanel::loadVideos(const QString& folderPath, QListWidget* listWidget) {
//    QDir dir(folderPath);
//    QStringList filters;
//#if defined(_WIN32)
//    filters << "*.wmv";
//#else
//    filters << "*.mp4" << "*.mov";
//#endif
//    dir.setNameFilters(filters);
//    dir.setFilter(QDir::Files);

//    listWidget->clear(); // 清空列表

//    for (const QString& fileName : dir.entryList()) {
//        QString filePath = dir.absoluteFilePath(fileName);

//        // 设置标题和默认时长
//        QString title = QFileInfo(fileName).fileName(); // 使用文件名作为标题

//        // 加载缩略图
//        QString thumbPath = filePath.left(filePath.lastIndexOf('.')) + ".png";
//        QPixmap thumbnail;
//        if (QFile::exists(thumbPath)) {
//            thumbnail.load(thumbPath);
//        } else {
//            thumbnail = QPixmap(":/images/default_thumbnail.png"); // 使用默认缩略图
//        }

//        // 加载视频时长
//        QString duration = "--:--"; // 默认时长
//        QMediaPlayer mediaPlayer;
//        mediaPlayer.setMedia(QUrl::fromLocalFile(filePath));
//        QEventLoop loop;
//        QObject::connect(&mediaPlayer, &QMediaPlayer::durationChanged, [&mediaPlayer, &duration, &loop](qint64 mediaDuration) {
//            duration = QTime(0, 0).addMSecs(mediaDuration).toString("mm:ss");
//            loop.quit();
//        });

//        // 等待元数据加载完成
//        mediaPlayer.play();
//        loop.exec();

//        // 创建自定义列表项
//        QListWidgetItem* item = new QListWidgetItem(listWidget);
//        PlayListItem* customItem = new PlayListItem;
//        customItem->updateInfo(title, thumbnail, duration, filePath); // 更新时传入视频路径

//        item->setSizeHint(customItem->sizeHint());
//        listWidget->setItemWidget(item, customItem);
//    }
//}

//// 在路径中插入零宽度空格，强制换行
//QString ListPanel::insertZeroWidthSpace(const QString& text) {
//    QString result;
//    for (int i = 0; i < text.length(); ++i) {
//        result.append(text[i]);
//        result.append("\u200B"); // 插入零宽度空格
//    }
//    return result;
//}

//ListPanel::ListPanel(QWidget *parent, QHBoxLayout *top, ThePlayer *player, ControlPanel *controlPanel) :
//    QWidget(parent),
//    ui(new Ui::ListPanel),
//    topLayout(top),
//    player(player),
//    controlPanel(controlPanel) // 初始化控制面板指针
//{
//    ui->setupUi(this);

//    this->setMaximumWidth(400);
//    ui->FavouriteOnButton->hide();
//    ui->CollectOnButton->hide();
//    ui->CommentOnButton->hide();
//    ui->CommentWidget->hide();
//    ui->PlaylistButton->hide();

//    PlayListItem* blankListItem = new PlayListItem();
//    ui->PlayingItemWidget->layout()->addWidget(blankListItem);
//    blankListItem->updateInfo("No video playing.", QPixmap(":/images/default_thumbnail.png"), 0, "No Path"); // 更新时传入视频路径

//    // 点击打开文件夹按钮时
//    QObject::connect(ui->OpenFolderButton, &QPushButton::clicked, [this]() {
//        QString folderPath = QFileDialog::getExistingDirectory(this, "Select Video Folder");
//        if (!folderPath.isEmpty()) {
//            loadVideos(folderPath, ui->PlaylistListWidget);
//            ui->CurrentPathLabel->setText(insertZeroWidthSpace(folderPath));
//        }
//    });

//    // 点击列表项，播放对应的视频，并把他添加到正在播放
//    QObject::connect(ui->PlaylistListWidget, &QListWidget::itemClicked, [&](QListWidgetItem* item) {
//        // 获取对应的自定义列表项
//        PlayListItem* customItem = qobject_cast<PlayListItem*>(ui->PlaylistListWidget->itemWidget(item));
//        if (customItem && player) {
//            QString videoPath = customItem->getVideoPath();

//            // 播放视频
//            player->playVideo(videoPath);

//            // 将当前播放的自定义列表项显示到 PlayingItemWidget
//            if (ui->PlayingItemWidget) {
//                // 先清除 PlayingItemWidget 中的内容
//                while (QLayoutItem* child = ui->PlayingItemWidget->layout()->takeAt(0)) {
//                    if (child->widget()) {
//                        delete child->widget();
//                    }
//                    delete child;
//                }

//                // 创建一个新的 Widget，复制 customItem 的信息
//                PlayListItem* playingItem = new PlayListItem;
//                playingItem->updateInfo(
//                    customItem->getTitle(),
//                    customItem->getThumbnail(),
//                    customItem->getDuration(),
//                    customItem->getVideoPath()
//                );

//                // 将复制的自定义列表项添加到 PlayingItemWidget
//                ui->PlayingItemWidget->layout()->addWidget(playingItem);
//            }
//        }
//    });


//    ui->CommentOnButton->setDisabled(true);
//    ui->PlaylistOnButton->setDisabled(true);
//    // 切换列表
//    QObject::connect(ui->CommentButton, &QPushButton::clicked, [&]() {
//        ui->CommentButton->hide();
//        ui->CommentOnButton->show();
//        ui->PlaylistButton->show();
//        ui->PlaylistOnButton->hide();
//        ui->CommentWidget->show();
//        ui->PlaylistWidget->hide();
//    });
//    QObject::connect(ui->PlaylistButton, &QPushButton::clicked, [&]() {
//        ui->CommentButton->show();
//        ui->CommentOnButton->hide();
//        ui->PlaylistButton->hide();
//        ui->PlaylistOnButton->show();
//        ui->CommentWidget->hide();
//        ui->PlaylistWidget->show();
//    });

//    // 关闭listPanel
//    QObject::connect(ui->CloseButton, &QPushButton::clicked, [&]() {
//        controlPanel->PlaylistButton->show();
//        controlPanel->PlaylistOpenButton->hide();
//        this->hide();
//    });

//}


//ListPanel::~ListPanel()
//{
//    delete ui;
//}

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

    // 点击列表项，播放对应的视频，并加载用户数据
    QObject::connect(ui->PlaylistListWidget, &QListWidget::itemClicked, [&](QListWidgetItem* item) {
        PlayListItem* customItem = qobject_cast<PlayListItem*>(ui->PlaylistListWidget->itemWidget(item));
        if (customItem && player) {
            currentVideoPath = customItem->getVideoPath();
            player->playVideo(currentVideoPath);

            // 更新点赞和收藏状态
            updateLikeButton(dataManager->getLikeStatus(currentVideoPath));
            updateCollectButton(dataManager->getCollectStatus(currentVideoPath));

            // 刷新评论列表
            refreshComments();

            // 启用按钮
            updateButtonStates(true);

            // 展示“正在播放”区域
            if (ui->PlayingItemWidget) {
                // 先清除 PlayingItemWidget 中的内容
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
    });

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

        connect(commentItem, &CommentListItem::editRequested, this, &ListPanel::onEditComment);
        connect(commentItem, &CommentListItem::deleteRequested, this, &ListPanel::onDeleteComment);

        QListWidgetItem *listItem = new QListWidgetItem(ui->CommentListWidget);
        listItem->setSizeHint(commentItem->sizeHint());
        ui->CommentListWidget->setItemWidget(listItem, commentItem);
    }
}

void ListPanel::onNewComment() {
    CommentDialog *dialog = new CommentDialog(this);
    dialog->setDialogMode(false); // 设置为新建模式
    dialog->setWindowModality(Qt::ApplicationModal); // 或 dialog->setModal(true);

    if (dialog->exec() == QDialog::Accepted) {
        QString username = dialog->getUsername();
        QString content = dialog->getCommentContent();

        if (!username.isEmpty() && !content.isEmpty()) {
            dataManager->addComment(currentVideoPath, username, content); // 添加评论
            refreshComments(); // 刷新评论列表
        }
    }

    delete dialog; // 释放对话框资源
}


void ListPanel::onEditComment(int commentId, const QString& newUsername, const QString& newContent) {
    dataManager->editComment(currentVideoPath, commentId, newUsername, newContent);
    refreshComments();
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

ListPanel::~ListPanel() {
    delete ui;
    if (dataManager) {
        delete dataManager;
    }
}
