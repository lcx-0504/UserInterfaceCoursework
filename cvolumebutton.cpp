#include "cvolumebutton.h"
#include <QCursor>
#include <QTimerEvent>
#include <QDebug>
CVolumeButton::CVolumeButton(QWidget *parent)
    : QPushButton(parent)
{
    // 在构造函数中初始化按钮样式，设置默认值等
}

void CVolumeButton::enterEvent(QEvent* event) {
    VolumeSlider->show();
    enterPos = QCursor::pos();

}
// 鼠标离开VolumeButton后，如果鼠标还在VolumeWidget内显示VolumeSlider，否则隐藏VolumeSlider
void CVolumeButton::leaveEvent(QEvent* event) {
    // 获取鼠标的全局坐标
    QPoint leavePos = QCursor::pos();
    // 判断鼠标有没有超出volumeWidget
    QWidget* volumeWidget = parentWidget();
    if (volumeWidget) {
        // 将鼠标的全局坐标转换为相对于VolumeWidget的坐标
        QPoint localPos = volumeWidget->mapFromGlobal(leavePos);
        // 获取了VolumeWidget相对于VolumeWidget的坐标
        QRect volumeWidgetRect(0, 0, volumeWidget->width(), volumeWidget->height());
        // qDebug()<<volumeWidgetRect.x();
        if (volumeWidgetRect.contains(localPos)) {
            VolumeSlider->show();
        } else {
            VolumeSlider->hide();
        }
    }
}
