#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QLabel>
#include <QListWidget>
#include <QFileDialog>
#include <QTime>
#include "the_player.h"
#include "control.h"
#include "playlist_item.h"

void loadVideos(const QString& folderPath, QListWidget* listWidget) {
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
QString insertZeroWidthSpace(const QString& text) {
    QString result;
    for (int i = 0; i < text.length(); ++i) {
        result.append(text[i]);
        result.append("\u200B"); // 插入零宽度空格
    }
    return result;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // the widget that will show the video
    // QVideoWidget is a control for displaying videos.
    // To use it, need to define a QMediaPlayer and set the VideoOutput of the QMediaPlayer to QVideoWidget
    QVideoWidget *videoWidget = new QVideoWidget;
    videoWidget->setStyleSheet("background-color: black;");  // 设置黑色背景
    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);


    // create the main window and layout
    QWidget window;
    QHBoxLayout *top = new QHBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    // left Panel
    QVBoxLayout *leftPanel = new QVBoxLayout();

    ControlPanel *controlPanel = new ControlPanel();

    controlPanel->PlayButton->hide();
    // 点击 PauseButton 时，隐藏 PlayButton
    controlPanel->connect(controlPanel->PauseButton, &QPushButton::clicked, [=]() {
        player->pause();
        controlPanel->PauseButton->hide();  // 隐藏 PlayButton
        controlPanel->PlayButton->show();
    });

    // 点击 PlayButton 时，隐藏 PauseButton
    controlPanel->connect(controlPanel->PlayButton, &QPushButton::clicked, [=]() {
        player->play();
        controlPanel->PlayButton->hide();  // 隐藏 PauseButton

        controlPanel->PauseButton->show();
    });

    // add the video and the buttons to the top level widget
    leftPanel->addWidget(videoWidget);
    leftPanel->addWidget(controlPanel);

    // right panel
    QWidget *rightPanelWidget = new QWidget(); //layout没有隐藏方法
    rightPanelWidget->setFixedWidth(300);
    QVBoxLayout *rightPanel = new QVBoxLayout(rightPanelWidget);

    QPushButton *openFolderButton = new QPushButton();
    openFolderButton->setText("Open Folder");
    QLabel *currentPathLabel = new QLabel();
    currentPathLabel->setText("Choose a folder.");
    currentPathLabel->setToolTip("Choose a folder.");
    currentPathLabel->setWordWrap(true);
    currentPathLabel->setFixedWidth(300);

    // 创建 QListWidget
    QListWidget* listWidget = new QListWidget();
    listWidget->setFixedWidth(300);


    rightPanel->addWidget(openFolderButton);
    rightPanel->addWidget(currentPathLabel);
    rightPanel->addWidget(listWidget);

    top->addLayout(leftPanel);
    top->addWidget(rightPanelWidget);


    // 显示和隐藏列表
    controlPanel->PlaylistButton->hide();
    QObject::connect(controlPanel->PlaylistButton, &QPushButton::clicked, [&]() {
        rightPanelWidget->show();
        controlPanel->PlaylistButton->hide();
        controlPanel->PlaylistOpenButton->show();
    });
    QObject::connect(controlPanel->PlaylistOpenButton, &QPushButton::clicked, [&]() {
        rightPanelWidget->hide();
        controlPanel->PlaylistButton->show();
        controlPanel->PlaylistOpenButton->hide();
    });


    // 检查启动参数
    if (argc == 2) {
        QString defaultPath = QString::fromStdString(argv[1]);
        if (QDir(defaultPath).exists()) {
            // 加载启动参数路径下的视频
            loadVideos(defaultPath, listWidget);
            currentPathLabel->setText(insertZeroWidthSpace(defaultPath)); // 显示当前路径（开启强制换行）
            currentPathLabel->setToolTip(defaultPath);
        } else {
            // 启动参数路径无效，弹出提示框
            QMessageBox::warning(&window, "Invalid Path", "The startup folder does not exist. Please select a valid folder.");
            currentPathLabel->setText("Invalid startup folder. Choose a folder."); // 提示用户选择路径
        }
    }

    // 点击打开文件夹按钮时，弹出文件夹选择对话框
    QObject::connect(openFolderButton, &QPushButton::clicked, [&]() {
        QString folderPath = QFileDialog::getExistingDirectory(&window, "Select Video Folder");
        if (!folderPath.isEmpty()) {
            // 加载选定路径下的视频
            loadVideos(folderPath, listWidget);
            currentPathLabel->setText(folderPath); // 显示当前路径
        }
    });

    // 点击列表项，播放对应的视频
    QObject::connect(listWidget, &QListWidget::itemClicked, [&player, listWidget](QListWidgetItem* item) {
        // 获取对应的自定义列表项
        playListItem* customItem = qobject_cast<playListItem*>(listWidget->itemWidget(item));
        if (customItem) {
            QString videoPath = customItem->getVideoPath();
            player->playVideo(videoPath); // 使用新方法直接播放视频
        }
    });


    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
