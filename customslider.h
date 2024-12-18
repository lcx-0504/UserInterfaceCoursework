#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H
#include <QSlider>
#include <QMouseEvent>
#include <QCoreApplication>

class CustomSlider : public QSlider
{
    Q_OBJECT
public:
    CustomSlider(QWidget *parent = 0) : QSlider(parent)
    {
    }
protected:
    void mousePressEvent(QMouseEvent *ev);
signals:
    void costomSliderClicked();// Custom mouse click signals for capture and processing
};

#endif // CUSTOMSLIDER_H

