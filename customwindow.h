#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include "control.h"
#include "controlvertical.h"
#include "listpanel.h"
#include "the_player.h"

class CustomWindow : public QWidget {
    Q_OBJECT

public:
    explicit CustomWindow(QWidget *parent = nullptr, char *argv[] = nullptr, int argc = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QHBoxLayout *top;
    QVBoxLayout *leftPanel;
    QVideoWidget *videoWidget;
    ThePlayer *player;
    ControlPanel *controlPanel;
    ControlPanelVertical *controlPanelVertical;
    ListPanel *listpanel;

    void setupUI();
    void connectSignals();
    void checkArguments(int argc, char *argv[]);
};

#endif // CUSTOMWINDOW_H
