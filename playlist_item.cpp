#include "playlist_item.h"
#include "ui_playlist_item.h"

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

QString playListItem::getTitle() const {
    return videoTitle;
}

QPixmap playListItem::getThumbnail() const {
    return videoThumbnail;
}

QString playListItem::getDuration() const {
    return videoDuration;
}
