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

void ThePlayer::shuffle() {
    // 如果需要随机播放的逻辑，可以实现此方法；目前暂时未使用
}

void ThePlayer::playStateChanged(QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::StoppedState:
            play(); // 视频结束后重新播放
            break;
        default:
            break;
    }
}
