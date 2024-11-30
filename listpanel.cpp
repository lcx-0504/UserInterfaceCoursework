#include "listpanel.h"
#include "ui_listpanel.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QTime>
#include "playlist_item.h"
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
        playListItem* customItem = new playListItem;
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

ListPanel::ListPanel(QWidget *parent, QHBoxLayout *top, ThePlayer *player, ControlPanel *controlPanel) :
    QWidget(parent),
    ui(new Ui::ListPanel),
    topLayout(top),
    player(player),
    controlPanel(controlPanel) // 初始化控制面板指针
{
    ui->setupUi(this);

    this->setMaximumWidth(400);
    ui->FavouriteOnButton->hide();
    ui->CollectOnButton->hide();
    ui->CommentOnButton->hide();
    ui->CommentWidget->hide();
    ui->PlaylistButton->hide();

    playListItem* blankListItem = new playListItem();
    ui->PlayingItemWidget->layout()->addWidget(blankListItem);
    blankListItem->updateInfo("No video playing.", QPixmap(":/images/default_thumbnail.png"), 0, "No Path"); // 更新时传入视频路径

    // 点击打开文件夹按钮时
    QObject::connect(ui->OpenFolderButton, &QPushButton::clicked, [this]() {
        QString folderPath = QFileDialog::getExistingDirectory(this, "Select Video Folder");
        if (!folderPath.isEmpty()) {
            loadVideos(folderPath, ui->PlaylistListWidget);
            ui->CurrentPathLabel->setText(insertZeroWidthSpace(folderPath));
        }
    });

    // 点击列表项，播放对应的视频，并把他添加到正在播放
    QObject::connect(ui->PlaylistListWidget, &QListWidget::itemClicked, [&](QListWidgetItem* item) {
        // 获取对应的自定义列表项
        playListItem* customItem = qobject_cast<playListItem*>(ui->PlaylistListWidget->itemWidget(item));
        if (customItem && player) {
            QString videoPath = customItem->getVideoPath();

            // 播放视频
            player->playVideo(videoPath);

            // 将当前播放的自定义列表项显示到 PlayingItemWidget
            if (ui->PlayingItemWidget) {
                // 先清除 PlayingItemWidget 中的内容
                while (QLayoutItem* child = ui->PlayingItemWidget->layout()->takeAt(0)) {
                    if (child->widget()) {
                        delete child->widget();
                    }
                    delete child;
                }

                // 创建一个新的 Widget，复制 customItem 的信息
                playListItem* playingItem = new playListItem;
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


    ui->CommentOnButton->setDisabled(true);
    ui->PlaylistOnButton->setDisabled(true);
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


ListPanel::~ListPanel()
{
    delete ui;
}
