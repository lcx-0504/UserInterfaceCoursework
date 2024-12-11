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
#include "listpanel.h"
#include "ui_listpanel.h"
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

    ControlPanel *controlPanel = new ControlPanel(&window, player);

    // add the video and the buttons to the top level widget
    leftPanel->addWidget(videoWidget);
    leftPanel->addWidget(controlPanel);

    // 右侧的listpanel
    ListPanel *listpanel = new ListPanel(&window, top, player, controlPanel);

    // 显示和隐藏列表
    controlPanel->PlaylistButton->hide();
    QObject::connect(controlPanel->PlaylistButton, &QPushButton::clicked, [&]() {
        listpanel->show();
        controlPanel->PlaylistButton->hide();
        controlPanel->PlaylistOpenButton->show();
    });
    QObject::connect(controlPanel->PlaylistOpenButton, &QPushButton::clicked, [&]() {
        listpanel->hide();
        controlPanel->PlaylistButton->show();
        controlPanel->PlaylistOpenButton->hide();
    });


    // 检查启动参数
    if (argc == 2) {
        QString defaultPath = QString::fromStdString(argv[1]);
        if (QDir(defaultPath).exists()) {
            // 加载启动参数路径下的视频
            listpanel->loadVideos(defaultPath, listpanel->ui->PlaylistListWidget);
            listpanel->ui->CurrentPathLabel->setText(listpanel->insertZeroWidthSpace(defaultPath)); // 显示当前路径（开启强制换行）
            listpanel->ui->CurrentPathLabel->setToolTip(defaultPath);
        } else {
            // 启动参数路径无效，弹出提示框
            QMessageBox::warning(&window, "Invalid Path", "The startup folder does not exist. Please select a valid folder.");
            listpanel->ui->CurrentPathLabel->setText("Invalid startup folder. Choose a folder."); // 提示用户选择路径
        }
    }

    top->addLayout(leftPanel);
    top->addWidget(listpanel);

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
