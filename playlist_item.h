#ifndef PLAY_LIST_ITEM_H
#define PLAY_LIST_ITEM_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QString>

namespace Ui {
class playListItem;
}

class playListItem : public QWidget
{
    Q_OBJECT

public:
    explicit playListItem(QWidget *parent = nullptr);
    ~playListItem();

    // 用于更新视频信息的方法
    void updateInfo(const QString& title, const QPixmap& thumbnail, const QString& duration, const QString& videoPath);

    // 获取存储的视频路径
    QString getVideoPath() const;

private:
    Ui::playListItem *ui;

    // 视频相关信息
    QString videoTitle;
    QPixmap videoThumbnail;
    QString videoDuration;
    QString videoPath; // 新增：存储视频文件路径
};

#endif // PLAY_LIST_ITEM_H
