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
    void enterEvent(QEvent* event) override;  // 覆盖 QEvent 类型的 enterEvent
    void leaveEvent(QEvent* event) override;

private:
    QSlider* VolumeSlider;
    QPoint enterPos;
    QPoint leavePos;
};

#endif // CVOLUMEBUTTON_H
