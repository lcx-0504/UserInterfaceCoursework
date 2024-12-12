#include "playlist_item.h"
#include "ui_playlist_item.h"

PlayListItem::PlayListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayListItem)
{
    ui->setupUi(this);
}

PlayListItem::~PlayListItem()
{
    delete ui;
}

void PlayListItem::updateInfo(const QString& title, const QPixmap& thumbnail, const QString& duration, const QString& videoPath) {
    videoTitle = title;
    videoThumbnail = thumbnail;
    videoDuration = duration;
    this->videoPath = videoPath;

    ui->title->setText(videoTitle);
    ui->label->setPixmap(videoThumbnail.scaled(100, 100, Qt::KeepAspectRatio)); // 缩放图片
    ui->time->setText(videoDuration);
}

QString PlayListItem::getVideoPath() const {
    return videoPath;
}

QString PlayListItem::getTitle() const {
    return videoTitle;
}

QPixmap PlayListItem::getThumbnail() const {
    return videoThumbnail;
}

QString PlayListItem::getDuration() const {
    return videoDuration;
}
