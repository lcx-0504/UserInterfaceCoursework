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

    // Methods for updating video information
    void updateInfo(const QString& title, const QPixmap& thumbnail, const QString& duration, const QString& videoPath);

    // Get method
    QString getVideoPath() const;
    QString getTitle() const;
    QPixmap getThumbnail() const;
    QString getDuration() const;

private:
    Ui::PlayListItem *ui;

    // Video related information
    QString videoTitle;
    QPixmap videoThumbnail;
    QString videoDuration;
    QString videoPath; // Path for storing video files
};

#endif // PLAY_LIST_ITEM_H
