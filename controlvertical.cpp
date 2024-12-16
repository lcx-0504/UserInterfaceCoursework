#include "controlvertical.h"

ControlPanelVertical::ControlPanelVertical(QWidget *parent, ThePlayer *playerInstance)
    : QWidget(parent),
    ui(new Ui::ControlPanelVertical), player(playerInstance)
{
    setupUi(this);
    FavouriteOnButton->hide();
    CollectOnButton->hide();
    CommentOnButton->hide();
    // 音量
    VolumeSlider->setMaximum(100);
    VolumeSlider->setMinimum(0);
    VolumeSlider->setValue(50);
    VolumeSlider->hide();
    VolumeOffButton->hide();
    VolumeButton->setVolumeSlider(VolumeSlider);
    VolumeWidget->setVolumeSlider(VolumeSlider);

    // 信号槽
    setupConnections();
}


void ControlPanelVertical::setPlayer(ThePlayer *playerInstance)
{
    player = playerInstance;
}

void ControlPanelVertical::setupConnections()
{
    connect(VolumeOffButton, &QPushButton::clicked, this, [=]() {
        int value = VolumeSlider->value();
        player->setVolume(value);
        VolumeButton->show();
        VolumeOffButton->hide();
    });

    // VolumeButton键
    connect(VolumeButton, &QPushButton::clicked, this, [=]() {
        player->setVolume(0);
        VolumeOffButton->show();
        VolumeButton->hide();
    });

    // VolumeSlider滑块
    connect(VolumeSlider, &QSlider::valueChanged, this, [=](int value) {
        player->setVolume(value);
    });
}
