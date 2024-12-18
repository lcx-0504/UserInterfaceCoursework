#ifndef CVOLUMESLIDER_H
#define CVOLUMESLIDER_H

#include <QSlider>
#include <QEvent>
class CVolumeSlider : public QSlider
{
    Q_OBJECT
public:
    explicit CVolumeSlider(QWidget *parent = nullptr);
protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
};

#endif // CVOLUMESLIDER_H
