#include "control.h"

ControlPanel::ControlPanel(QWidget *parent, ThePlayer *playerInstance)
    : QWidget(parent), player(playerInstance)
{
    setupUi(this);
    PlayButton->hide();
    PlaylistButton->hide();
    // volume
    VolumeSlider->setMaximum(100);
    VolumeSlider->setMinimum(0);
    VolumeSlider->setValue(50);
    VolumeSlider->hide();
    VolumeOffButton->hide();
    VolumeButton->setVolumeSlider(VolumeSlider);
    VolumeWidget->setVolumeSlider(VolumeSlider);
    // The TimeLable is not displayed initially
    TimeLabel->setText("");
    // Signal slot
    setupConnections();
    FullscreenExitButton->hide();
}


void ControlPanel::setPlayer(ThePlayer *playerInstance)
{
    player = playerInstance;
}
void ControlPanel::handleSignal(int value){
    if(value == 2){
        if(!this->ForwardButton->isHidden()){
            PauseButton->show();
            PlayButton->hide();
        }
    }else if(value == 1){
        if(!this->ForwardButton->isHidden()){
            PauseButton->hide();
            PlayButton->show();
        }
    }
}
void ControlPanel::setupConnections()
{
    // When you click PauseButton, hide the PlayButton
    connect(PauseButton, &QPushButton::clicked, this, [=]() {
        player->pause();
    });

    // When you click PlayButton, hide the PauseButton
    connect(PlayButton, &QPushButton::clicked, this, [=]() {
        player->play();
    });
    // VolumeOffButton
    connect(VolumeOffButton, &QPushButton::clicked, this, [=]() {
        int value = VolumeSlider->value();
        player->setVolume(value);
        VolumeWidget->show();
        VolumeOffButton->hide();
    });

    // VolumeButton
    connect(VolumeButton, &QPushButton::clicked, this, [=]() {
        player->setVolume(0);
        VolumeOffButton->show();
        VolumeWidget->hide();
    });

    // VolumeSlider
    connect(VolumeSlider, &QSlider::valueChanged, this, [=](int value) {
        player->setVolume(value);
    });

    // Video length change
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(onPlayerDurationChanged(qint64)));
    // The position of the progress bar changes
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(onPlayerPositionChanged(qint64)));
    // Drag the progress bar to change the progress of the video
    connect(ProgressBar,SIGNAL(sliderMoved(int)),this,SLOT(onSliderMoved(int)));
    // Click on the progress bar and the video moves directly to that location
    connect(ProgressBar,SIGNAL(costomSliderClicked()),this,SLOT(sliderClicked()));

    // Click ForwardButton to fast forward 5 seconds
   connect(ForwardButton, &QPushButton::clicked, this, [=]() {
       qint64 currentPosition = player->position();
       player->setPosition(currentPosition + 5000);  //Fast forward five seconds
   });

   // Click the BackwardButton to rewind for 5 seconds
   connect(BackwardButton, &QPushButton::clicked, this, [=]() {
       qint64 currentPosition = player->position();
       player->setPosition(currentPosition - 5000);  // Get back five seconds
   });
}

// Set the maximum progress bar to the total video length
void ControlPanel::onPlayerDurationChanged(qint64 duration){
//    qDebug()<<duration;
    ProgressBar->setMaximum(duration);
}

// Get video progress
void ControlPanel::onPlayerPositionChanged(qint64 position){
//    qDebug()<<position;
    ProgressBar->setValue(position);
    updateTimeLabel(position);
}

// Change the progress of the video according to the value of the progress bar
void ControlPanel::onSliderMoved(int value){
    player->setPosition(value);
}

// After the slider click event occurs, change the progress of the video
void ControlPanel::sliderClicked(){
    player->setPosition(ProgressBar->value());
}

// Call the formatTime function to update the time
void ControlPanel::updateTimeLabel(qint64 position) {
    qint64 duration = player->duration();
    QString currentTime = formatTime(position);
    QString totalTime = formatTime(duration);
    TimeLabel->setText(currentTime + " / " + totalTime);
}

// 格式化时间为"MM:SS / MM:SS"
QString ControlPanel::formatTime(qint64 timeInMilliseconds) {
    int totalSeconds = timeInMilliseconds / 1000;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return QString::asprintf(" %02d:%02d ", minutes, seconds);
}
