#ifndef CVOLUMEBUTTON_H
#define CVOLUMEBUTTON_H

#include <QPushButton>
#include <QSlider>
#include <QEvent>
class CVolumeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CVolumeButton(QWidget *parent = nullptr);
    void setVolumeSlider(QSlider* slider) { VolumeSlider = slider; }
protected:
    void enterEvent(QEvent* event) override;  // Overrides enterEvent of type QEvent
    void leaveEvent(QEvent* event) override;

private:
    QSlider* VolumeSlider;
    QPoint enterPos;
    QPoint leavePos;
};

#endif // CVOLUMEBUTTON_H
