#ifndef THE_VIDEOWIDGET_H
#define THE_VIDEOWIDGET_H
#include <QApplication>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QWidget>
#include "the_player.h"
class VideoWidget : public QVideoWidget {
    Q_OBJECT
public:
    VideoWidget(ThePlayer *player, QWidget *parent = nullptr)
        : QVideoWidget(parent), m_player(player) {}

protected:
    void mousePressEvent(QMouseEvent *event) override {
        // Switch between playing and pausing
        if (m_player->state() == QMediaPlayer::PlayingState) {
            m_player->pause();
        } else if (m_player->state() == QMediaPlayer::PausedState || m_player->state() == QMediaPlayer::StoppedState) {
            m_player->play();
        }
        QVideoWidget::mousePressEvent(event); // Keep default behavior
    }

private:
    ThePlayer *m_player;
};
#endif // THE_VIDEOWIDGET_H
