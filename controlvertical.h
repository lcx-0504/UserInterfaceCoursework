#ifndef CONTROL_PANEL_VERTICAL_H
#define CONTROL_PANEL_VERTICAL_H

#include <QWidget>
#include "the_player.h"
#include "ui_controlvertical.h"

namespace Ui {
class ControlPanelVertical;
}

class ControlPanelVertical : public QWidget, public Ui::ControlPanelVertical
{
    Q_OBJECT

public:
    Ui::ControlPanelVertical *ui;
    explicit ControlPanelVertical(QWidget *parent = nullptr, ThePlayer *playerInstance = nullptr);
    void setPlayer(ThePlayer *playerInstance);
private:
    ThePlayer *player;
    void setupConnections();

};

#endif // CONTROL_PANEL_VERTICAL_H
