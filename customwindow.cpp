#include "customwindow.h"
#include <QResizeEvent>
#include <QPushButton>

CustomWindow::CustomWindow(QWidget *parent, char *argv[], int argc)
    : QWidget(parent) {
    // Set up the UI
    setupUI();

    // Check for command line arguments
    checkArguments(argc, argv);

    // Connect signals and slots
    connectSignals();
}

void CustomWindow::setupUI() {
    // Video widget and player
    videoWidget = new QVideoWidget;
    videoWidget->setStyleSheet("background-color: black;");
    player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // Layouts
    top = new QHBoxLayout();
    top->setStretch(1, 0);
    setLayout(top);
    setWindowTitle("tomeo");
    setMinimumSize(800, 680);

    // Left Panel
    leftPanel = new QVBoxLayout();
    controlPanel = new ControlPanel(this, player);
    controlPanelVertical = new ControlPanelVertical(this, player);
    leftPanel->addWidget(videoWidget);
    leftPanel->addWidget(controlPanel);

    // Right List Panel
    listpanel = new ListPanel(this, top, player, controlPanel, controlPanelVertical);

    // Add layouts to top layout
    top->addLayout(leftPanel);
    top->addWidget(controlPanelVertical);
    top->addWidget(listpanel);
}

void CustomWindow::connectSignals() {
    // Show/hide list panel when buttons are clicked
    controlPanel->PlaylistOpenButton->hide();
    connect(controlPanel->PlaylistButton, &QPushButton::clicked, [this]() {
        listpanel->show();
        controlPanel->PlaylistButton->hide();
        controlPanel->PlaylistOpenButton->show();
        controlPanelVertical->PlaylistButton->hide();
        controlPanelVertical->PlaylistOpenButton->show();
    });
    // 关闭右侧板
    connect(controlPanel->PlaylistOpenButton, &QPushButton::clicked, [this]() {
        listpanel->hide();
        controlPanel->PlaylistButton->show();
        controlPanel->PlaylistOpenButton->hide();
    });

    controlPanelVertical->PlaylistOpenButton->hide();
    connect(controlPanelVertical->PlaylistButton, &QPushButton::clicked, [this]() {
        listpanel->show();
        controlPanelVertical->PlaylistButton->hide();
        controlPanelVertical->PlaylistOpenButton->show();
    });
    connect(controlPanelVertical->PlaylistOpenButton, &QPushButton::clicked, [this]() {
        listpanel->hide();
        controlPanelVertical->PlaylistButton->show();
        controlPanelVertical->PlaylistOpenButton->hide();
    });
}

void CustomWindow::checkArguments(int argc, char *argv[]) {
    if (argc == 2) {
        QString defaultPath = QString::fromStdString(argv[1]);
        listpanel->openFolder(defaultPath); // Open folder if argument provided
    }
}

void CustomWindow::resizeEvent(QResizeEvent *event) {
    // Check if the event type is resize
    if (event->type() == QEvent::Resize) {
        // Get the new width of the window
        int width = event->size().width();

        // Check if the width is larger than 900
        if (width > 900) {
            // Larger than 900, show controlPanel, hide controlPanelVertical, show listPanel
            controlPanel->BackwardButton->show();
            controlPanel->ForwardButton->show();
            controlPanel->FullscreenButton->show();
            controlPanel->SkipPrevButton->show();
            controlPanel->SkipNextButton->show();
            controlPanel->PlaylistOpenButton->show();
            // Check if the volume is 0 and show VolumeOffButton accordingly
            if (player->volume() == 0) {
                controlPanel->VolumeOffButton->show();
            } else {
                controlPanel->VolumeWidget->show();
            }
            // 要改
            controlPanel->PauseButton->show();
            listpanel->show();
            controlPanelVertical->hide();
        } else {
            //Smaller than 900, hide controlPanel, show controlPanelVertical, hide listPanel
            int count = controlPanel->horizontalLayout->count();
            for (int i = 0; i < count; ++i) {
                QWidget *widget = controlPanel->horizontalLayout->itemAt(i)->widget();
                if (widget) {
                    widget->hide();  // Hide widgets in controlPanel
                }
            }
            listpanel->hide();  // Hide the list panel
            controlPanelVertical->show();  // Show the vertical control panel
        }
    }

    QWidget::resizeEvent(event);
}
