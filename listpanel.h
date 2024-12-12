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

    void refreshComments();      // 刷新评论列表
    void updateLikeButton(bool liked);
    void updateCollectButton(bool collected);
    void updateButtonStates(bool enabled);

private slots:
    void onNewComment();
    void onEditComment(int commentId, const QString& newUsername, const QString& newContent);
    void onDeleteComment(int commentId);
    void onLikeClicked();
    void onCollectClicked();
};

#endif // LISTPANEL_H
