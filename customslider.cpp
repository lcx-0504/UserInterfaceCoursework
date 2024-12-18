#include "customslider.h"

void CustomSlider::mousePressEvent(QMouseEvent *event)
{
    //Note that the mouse click of the parent class should be called first to handle the event, so that the dragging situation is not affected
    QSlider::mousePressEvent(event);
    //Get the mouse position, which cannot be taken directly from ev (because if you drag, the mouse starts clicking at a meaningless position)
    double pos = event->pos().x() / (double)width();
    setValue(pos * (maximum() - minimum()) + minimum());
    // Send a custom mouse click signal
    emit costomSliderClicked();
}

