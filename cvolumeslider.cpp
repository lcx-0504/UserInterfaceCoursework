#include "cvolumeslider.h"
#include <QCursor>
#include <QTimerEvent>
#include <QDebug>
CVolumeSlider::CVolumeSlider(QWidget *parent)
    : QSlider(parent)
{
    // 在构造函数中初始化按钮样式，设置默认值等
}

void CVolumeSlider::enterEvent(QEvent* event) {
    this->show();
}

void CVolumeSlider::leaveEvent(QEvent* event) {
    this->hide();
}
