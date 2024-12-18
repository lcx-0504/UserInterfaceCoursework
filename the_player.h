#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H

#include <QApplication>
#include <QMediaPlayer>
#include <vector>
#include <QTimer>
#include <QMouseEvent>
class ControlPanel;
class ThePlayer : public QMediaPlayer {
    Q_OBJECT

private:
    bool isManualStop = false; // Indicates whether to manually switch the video for the user (the trigger signal is required only when the automatic play ends

public:
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(50); // Default volume
        connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onMediaStateChanged(QMediaPlayer::State)));
    }

    // Set video path
    void playVideo(const QString& videoPath);
    void pause();
    void play();

    void stopManually(); // Set the user to manually switch the video

private slots:
    void onMediaStateChanged(QMediaPlayer::State ms);

signals:
    void playbackFinished();
    void change_state(int value);
};

#endif //CW2_THE_PLAYER_H
