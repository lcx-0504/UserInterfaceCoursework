#include "cvolumewidget.h"

CVolumeWidget::CVolumeWidget(QWidget *parent)
    : QWidget{parent}
{}

void CVolumeWidget::leaveEvent(QEvent* event) {
    VolumeSlider->hide();
}
