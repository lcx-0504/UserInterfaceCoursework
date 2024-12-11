#ifndef CVOLUMEWIDGET_H
#define CVOLUMEWIDGET_H
#include <QEvent>
#include <QWidget>
#include <QSlider>
class CVolumeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CVolumeWidget(QWidget *parent = nullptr);
    void setVolumeSlider(QSlider* slider) { VolumeSlider = slider; }
protected:
    void leaveEvent(QEvent* event) override;

private:
    QSlider* VolumeSlider;
};

#endif // CVOLUMEWIDGET_H
