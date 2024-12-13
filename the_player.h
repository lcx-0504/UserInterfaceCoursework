#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H

#include <QApplication>
#include <QMediaPlayer>
#include <vector>
#include <QTimer>
#include <QMouseEvent>
class ThePlayer : public QMediaPlayer {
    Q_OBJECT

private:
    bool isManualStop = false; // 标志是否为用户手动切换视频(只有自动播放到结束才需要触发信号)

public:
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(50); // 默认音量
        connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onMediaStateChanged(QMediaPlayer::State)));
    }

    // 设置视频路径
    void playVideo(const QString& videoPath);
    void pause();
    void play();

    void stopManually(); // 设置用户是手动切换了视频

private slots:
    void onMediaStateChanged(QMediaPlayer::State ms);

signals:
    void playbackFinished();

};

#endif //CW2_THE_PLAYER_H
