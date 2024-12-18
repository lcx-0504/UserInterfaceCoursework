#include "the_player.h"
#include <QUrl>

void ThePlayer::playVideo(const QString& videoPath) {
    setMedia(QUrl::fromLocalFile(videoPath)); // Set video path
    play(); // start playing
}

void ThePlayer::pause() {
    QMediaPlayer::pause(); // Call the pause method of the base class QMediaPlayer
}

void ThePlayer::play() {
    QMediaPlayer::play(); // Call the play method of the base class QMediaPlayer
}

void ThePlayer::stopManually() {
    isManualStop = true;
    stop(); // Call the stop method of the base class
}

void ThePlayer::onMediaStateChanged(QMediaPlayer::State state) {
    if(state == QMediaPlayer::StoppedState || QMediaPlayer::PausedState){
        change_state(1);
    }
    if(state == QMediaPlayer::PlayingState){
        change_state(2);
    }
    if (state == QMediaPlayer::StoppedState && !isManualStop) {
        emit playbackFinished(); // A playback completion signal is sent only when not manually stopped
    }
    isManualStop = false; // reset mark
}
