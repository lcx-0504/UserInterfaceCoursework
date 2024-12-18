#include "cvolumebutton.h"
#include <QCursor>
#include <QTimerEvent>
#include <QDebug>
CVolumeButton::CVolumeButton(QWidget *parent)
    : QPushButton(parent)
{
    // Initialize button styles in constructors, set default values, and more
}

void CVolumeButton::enterEvent(QEvent* event) {
    VolumeSlider->show();
    enterPos = QCursor::pos();

}
// After the mouse leaves the VolumeButton, if the mouse still displays VolumeSlider inside the VolumeWidget, hide VolumeSlider otherwise
void CVolumeButton::leaveEvent(QEvent* event) {
    // Gets the global coordinates of the mouse
    QPoint leavePos = QCursor::pos();
    // Determine if the mouse has exceeded volumeWidget
    QWidget* volumeWidget = parentWidget();
    if (volumeWidget) {
        // Converts the global coordinates of the mouse to those relative to VolumeWidget
        QPoint localPos = volumeWidget->mapFromGlobal(leavePos);
        // Gets the coordinates of VolumeWidget with respect to VolumeWidget
        QRect volumeWidgetRect(0, 0, volumeWidget->width(), volumeWidget->height());
        // qDebug()<<volumeWidgetRect.x();
        if (volumeWidgetRect.contains(localPos)) {
            VolumeSlider->show();
        } else {
            VolumeSlider->hide();
        }
    }
}
