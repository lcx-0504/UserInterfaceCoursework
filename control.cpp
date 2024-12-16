#include "control.h"

ControlPanel::ControlPanel(QWidget *parent, ThePlayer *playerInstance)
    : QWidget(parent), player(playerInstance)
{
    setupUi(this);
    PlayButton->hide();
    PlaylistButton->hide();
    // 音量
    VolumeSlider->setMaximum(100);
    VolumeSlider->setMinimum(0);
    VolumeSlider->setValue(50);
    VolumeSlider->hide();
    VolumeOffButton->hide();
    VolumeButton->setVolumeSlider(VolumeSlider);
    VolumeWidget->setVolumeSlider(VolumeSlider);
    // 一开始不显示TimeLable
    TimeLable->setText("");
    // 信号槽
    setupConnections();
}


void ControlPanel::setPlayer(ThePlayer *playerInstance)
{
    player = playerInstance;
}

void ControlPanel::setupConnections()
{
    // 点击 PauseButton 时，隐藏 PlayButton
    connect(PauseButton, &QPushButton::clicked, this, [=]() {
        player->pause();
        PauseButton->hide();
        PlayButton->show();
    });

    // 点击 PlayButton 时，隐藏 PauseButton
    connect(PlayButton, &QPushButton::clicked, this, [=]() {
        player->play();
        PlayButton->hide();
        PauseButton->show();
    });
    // VolumeOffButton键
    connect(VolumeOffButton, &QPushButton::clicked, this, [=]() {
        int value = VolumeSlider->value();
        player->setVolume(value);
        VolumeWidget->show();
        VolumeOffButton->hide();
    });

    // VolumeButton键
    connect(VolumeButton, &QPushButton::clicked, this, [=]() {
        player->setVolume(0);
        VolumeOffButton->show();
        VolumeWidget->hide();
    });

    // VolumeSlider滑块
    connect(VolumeSlider, &QSlider::valueChanged, this, [=](int value) {
        player->setVolume(value);
    });

    //视频长度改变
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(onPlayerDurationChanged(qint64)));
    //进度条位置改变
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(onPlayerPositionChanged(qint64)));
    //拖动进度条，改变视频的进度
    connect(ProgressBar,SIGNAL(sliderMoved(int)),this,SLOT(onSliderMoved(int)));
    // 点击进度条，视频直接移到那个位置
    connect(ProgressBar,SIGNAL(costomSliderClicked()),this,SLOT(sliderClicked()));

    // 点击 ForwardButton 快进 5 秒
   connect(ForwardButton, &QPushButton::clicked, this, [=]() {
       qint64 currentPosition = player->position();
       player->setPosition(currentPosition + 5000);  // 快进5秒
   });

   // 点击 BackwardButton 快退 5 秒
   connect(BackwardButton, &QPushButton::clicked, this, [=]() {
       qint64 currentPosition = player->position();
       player->setPosition(currentPosition - 5000);  // 快退5秒
   });
}

// 设置进度条最大值为视频总长度
void ControlPanel::onPlayerDurationChanged(qint64 duration){
//    qDebug()<<duration;
    ProgressBar->setMaximum(duration);
}

// 获取视频进度
void ControlPanel::onPlayerPositionChanged(qint64 position){
//    qDebug()<<position;
    ProgressBar->setValue(position);
    updateTimeLabel(position);
}

// 根据进度条的value，改变视频的进度
void ControlPanel::onSliderMoved(int value){
    player->setPosition(value);
}

// slider点击事件发生后，改变视频的进度
void ControlPanel::sliderClicked(){
    player->setPosition(ProgressBar->value());
}

// 调用formatTime函数更新时间
void ControlPanel::updateTimeLabel(qint64 position) {
    qint64 duration = player->duration();
    QString currentTime = formatTime(position);
    QString totalTime = formatTime(duration);
    TimeLable->setText(currentTime + " / " + totalTime);
}

// 格式化时间为"MM:SS / MM:SS"
QString ControlPanel::formatTime(qint64 timeInMilliseconds) {
    int totalSeconds = timeInMilliseconds / 1000;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return QString::asprintf("%02d:%02d", minutes, seconds);
}
