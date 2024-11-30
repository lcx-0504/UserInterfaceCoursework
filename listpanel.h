#ifndef LISTPANEL_H
#define LISTPANEL_H

#include <QWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include "the_player.h"
#include "control.h" // 包含 ControlPanel 的声明

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
    ~ListPanel();

private:
    QHBoxLayout *topLayout;
    ThePlayer *player; // 存储播放器指针
    ControlPanel *controlPanel; // 存储控制面板指针
};

#endif // LISTPANEL_H
