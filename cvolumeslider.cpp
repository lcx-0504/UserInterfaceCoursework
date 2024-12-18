#include "cvolumeslider.h"
#include <QCursor>
#include <QTimerEvent>
#include <QDebug>
CVolumeSlider::CVolumeSlider(QWidget *parent)
    : QSlider(parent)
{
    // Initialize button styles in constructors, set default values, and more
}

void CVolumeSlider::enterEvent(QEvent* event) {
    this->show();
}

void CVolumeSlider::leaveEvent(QEvent* event) {
    this->hide();
}
