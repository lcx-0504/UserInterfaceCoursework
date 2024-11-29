#include "playlist_item.h"
#include "ui_play_list_item.h"

playListItem::playListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::playListItem)
{
    ui->setupUi(this);
}

playListItem::~playListItem()
{
    delete ui;
}

void playListItem::updateInfo(const QString& title, const QPixmap& thumbnail, const QString& duration, const QString& videoPath) {
    videoTitle = title;
    videoThumbnail = thumbnail;
    videoDuration = duration;
    this->videoPath = videoPath;

    ui->title->setText(videoTitle);
    ui->label->setPixmap(videoThumbnail.scaled(100, 100, Qt::KeepAspectRatio)); // 缩放图片
    ui->time->setText(videoDuration);
}

QString playListItem::getVideoPath() const {
    return videoPath;
}
