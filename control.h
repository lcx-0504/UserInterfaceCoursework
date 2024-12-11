#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include "the_player.h"
#include "ui_control.h"

class ControlPanel : public QWidget, public Ui::ControlPanel
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr, ThePlayer *playerInstance = nullptr);
    void setPlayer(ThePlayer *playerInstance);
private:
    ThePlayer *player;
    void setupConnections();


private slots:
    // 控制进度条
    void onPlayerDurationChanged(qint64 duration);
    void onPlayerPositionChanged(qint64 position);
    void onSliderMoved(int value);
    void sliderClicked();

    // 显示时间
    void updateTimeLabel(qint64 position);
    QString formatTime(qint64 timeInMilliseconds);

};

#endif // CONTROL_PANEL_H
