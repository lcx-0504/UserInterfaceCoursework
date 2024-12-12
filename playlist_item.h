#ifndef PLAY_LIST_ITEM_H
#define PLAY_LIST_ITEM_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QString>

namespace Ui {
class PlayListItem;
}

class PlayListItem : public QWidget
{
    Q_OBJECT

public:
    explicit PlayListItem(QWidget *parent = nullptr);
    ~PlayListItem();

    // 更新视频信息的方法
    void updateInfo(const QString& title, const QPixmap& thumbnail, const QString& duration, const QString& videoPath);

    // Get方法
    QString getVideoPath() const;
    QString getTitle() const;
    QPixmap getThumbnail() const;
    QString getDuration() const;

private:
    Ui::PlayListItem *ui;

    // 视频相关信息
    QString videoTitle;
    QPixmap videoThumbnail;
    QString videoDuration;
    QString videoPath; // 存储视频文件路径
};

#endif // PLAY_LIST_ITEM_H
