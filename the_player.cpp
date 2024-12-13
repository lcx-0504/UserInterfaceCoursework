#include "the_player.h"
#include <QUrl>

void ThePlayer::playVideo(const QString& videoPath) {
    setMedia(QUrl::fromLocalFile(videoPath)); // 设置视频路径
    play(); // 开始播放
}

void ThePlayer::pause() {
    QMediaPlayer::pause(); // 调用基类 QMediaPlayer 的 pause 方法
}

void ThePlayer::play() {
    QMediaPlayer::play(); // 调用基类 QMediaPlayer 的 play 方法
}

void ThePlayer::stopManually() {
    isManualStop = true;
    stop(); // 调用基类的停止方法
}

void ThePlayer::onMediaStateChanged(QMediaPlayer::State state) {
    if (state == QMediaPlayer::StoppedState && !isManualStop) {
        emit playbackFinished(); // 仅在非手动停止时发送播放完成信号
    }
    isManualStop = false; // 重置标志
}
