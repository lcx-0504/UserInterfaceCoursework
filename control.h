#ifndef CONTROL
#define CONTROL

#include <QWidget>
#include "ui_control.h"
#include "the_player.h"
class ControlPanel : public QWidget, public Ui::ControlPanel
{
    Q_OBJECT

public:
    ControlPanel(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setupUi(this);
//        setStyleSheet("background-color: green;");  just for test the position
    }
};

#endif // CONTROL
