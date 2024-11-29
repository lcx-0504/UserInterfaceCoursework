#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H

#include <QApplication>
#include <QMediaPlayer>
#include <vector>
#include <QTimer>

class ThePlayer : public QMediaPlayer {
    Q_OBJECT

private:
    QTimer* mTimer;
    long updateCount = 0;

public:
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0); // 默认音量
        connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(playStateChanged(QMediaPlayer::State)));

        mTimer = new QTimer(NULL);
        mTimer->setInterval(1000); // 每秒调用一次 shuffle
        mTimer->start();
        connect(mTimer, SIGNAL(timeout()), SLOT(shuffle()));
    }

    // 设置视频路径
    void playVideo(const QString& videoPath);
    void pause();
    void play();

private slots:
    void shuffle(); // 当前未使用，可移除或保留
    void playStateChanged(QMediaPlayer::State ms);

};

#endif //CW2_THE_PLAYER_H
