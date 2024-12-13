#ifndef LISTPANEL_H
#define LISTPANEL_H

#include <QWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QVBoxLayout>
#include "the_player.h"
#include "control.h"
#include "datamanager.h"
#include "commentlist_item.h"

namespace Ui {
class ListPanel;
}

class ListPanel : public QWidget
{
    Q_OBJECT

public:
    Ui::ListPanel *ui;
    explicit ListPanel(QWidget *parent = nullptr, QHBoxLayout *top = nullptr, ThePlayer *player = nullptr, ControlPanel *controlPanel = nullptr);
    void loadVideos(const QString& folderPath, QListWidget* listWidget);
    QString insertZeroWidthSpace(const QString& text);
    void openFolder(const QString& folderPath);
    ~ListPanel();

private:

    QHBoxLayout *topLayout;
    ThePlayer *player;
    ControlPanel *controlPanel;

    QString currentVideoPath;    // 当前播放的视频路径
    DataManager *dataManager;    // 管理用户数据

    // 播放视频
    void playSelectedVideo();

    void refreshComments();      // 刷新评论列表
    void updateLikeButton(bool liked);
    void updateCollectButton(bool collected);
    void updateButtonStates(bool enabled);

    // 播放模式
    enum PlayMode { ListLoop, SingleLoop, RandomLoop, NoLoop };
    PlayMode currentPlayMode;
    void onPlayModeButtonClicked();
    void updatePlayModeUI();
    void handlePlaybackFinished();

private slots:
    void onNewComment();
    void onEditCommentRequested(int commentId, const QString &username, const QString &content);
    void onDeleteComment(int commentId);
    void onLikeClicked();
    void onCollectClicked();

    void playNextVideo();     // 播放下一个视频
    void playPreviousVideo(); // 播放上一个视频
};

#endif // LISTPANEL_H
