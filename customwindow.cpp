#include "customwindow.h"
#include <QResizeEvent>
#include <QPushButton>
#include "the_videowidget.h"
#include "ui_listpanel.h"
int screen_mode = 0;
int lang_mode = 0;
CustomWindow::CustomWindow(QWidget *parent, char *argv[], int argc)
    : QWidget(parent) {
    // Set up the UI
    setupUI();
    qApp->installTranslator(&translator);
    // Check for command line arguments
    checkArguments(argc, argv);

    // Connect signals and slots
    connectSignals();
}

void CustomWindow::setupUI() {
    // Video widget and player

    player = new ThePlayer;
    videoWidget = new VideoWidget(player);
    videoWidget->setStyleSheet("background-color: black;");
    player->setVideoOutput(videoWidget);

    // Layouts
    top = new QHBoxLayout();
    top->setStretch(1, 0);
    top->setSpacing(12);

    setWindowTitle("tomeo");
    setMinimumSize(400, 680);
    this->resize(QSize(500, 680));
    // Left Panel
    leftPanel = new QVBoxLayout();
    updown = new QStackedLayout();

    controlPanel = new ControlPanel(this, player);
    controlPanelVertical = new ControlPanelVertical(this, player);
    leftPanel->addWidget(videoWidget);
    leftPanel->addWidget(controlPanel);
    leftPanel->setStretch(1,0);

    // Right List Panel
    listpanel = new ListPanel(this, top, player, controlPanel, controlPanelVertical);
    // set_control
    connect(player, &ThePlayer::change_state, controlPanel, &ControlPanel::handleSignal);
    connect(controlPanel->VolumeSlider, &QSlider::valueChanged, controlPanelVertical->VolumeSlider, &QSlider::setValue);
    connect(controlPanelVertical->VolumeSlider, &QSlider::valueChanged, controlPanel->VolumeSlider, &QSlider::setValue);
    connect(controlPanel->VolumeButton, &CVolumeButton::clicked, controlPanelVertical->VolumeButton, &CVolumeButton::hide);
    connect(controlPanel->VolumeButton, &CVolumeButton::clicked, controlPanelVertical->VolumeOffButton, &QPushButton::show);
    connect(controlPanel->VolumeOffButton, &QPushButton::clicked, controlPanelVertical->VolumeButton, &CVolumeButton::show);
    connect(controlPanel->VolumeOffButton, &QPushButton::clicked, controlPanelVertical->VolumeOffButton, &QPushButton::hide);

    // Add layouts to top layout

    top->addLayout(leftPanel);
    top->addWidget(controlPanelVertical);
    top->addWidget(listpanel);
    controlPanel->FullscreenExitButton->hide();
    setLayout(top);
}

void CustomWindow::connectSignals() {
    // Show/hide list panel when buttons are clicked

    controlPanel->PlaylistOpenButton->hide();
    connect(controlPanelVertical->PlaylistButton2, &QPushButton::clicked, [this](){
        videoWidget->hide();
        controlPanel->hide();
        controlPanelVertical->hide();
        controlPanelVertical->PlaylistButton->hide();
        controlPanelVertical->PlaylistOpenButton->show();
    });
    connect(controlPanelVertical->PlaylistOnButton, &QPushButton::clicked, [this](){
        videoWidget->hide();
        controlPanel->hide();
        listpanel->show();
        controlPanelVertical->hide();
        controlPanelVertical->PlaylistButton->hide();
        controlPanelVertical->PlaylistOpenButton->show();
    });
    connect(controlPanelVertical->CommentOnButton, &QPushButton::clicked, [this](){
        videoWidget->hide();
        controlPanel->hide();
        listpanel->show();
        controlPanelVertical->hide();
        controlPanelVertical->PlaylistButton->hide();
        controlPanelVertical->PlaylistOpenButton->show();
    });
    connect(controlPanelVertical->CommentButton, &QPushButton::clicked, [this](){
        videoWidget->hide();
        controlPanel->hide();
        controlPanelVertical->hide();
        controlPanelVertical->PlaylistButton->hide();
        controlPanelVertical->PlaylistOpenButton->show();
    });
    connect(controlPanel->PlaylistButton, &QPushButton::clicked, [this]() {
        listpanel->show();
        controlPanel->PlaylistButton->hide();
        controlPanel->PlaylistOpenButton->show();
        controlPanelVertical->PlaylistButton->hide();
        controlPanelVertical->PlaylistOpenButton->show();
    });
    // Display corresponding panel

    // Close right panel
    connect(controlPanel->PlaylistOpenButton, &QPushButton::clicked, [this]() {
        listpanel->hide();
        controlPanel->PlaylistButton->show();
        controlPanel->PlaylistOpenButton->hide();
    });

    controlPanelVertical->PlaylistOpenButton->hide();
    connect(controlPanelVertical->PlaylistButton, &QPushButton::clicked, [this]() {
        listpanel->show();
        videoWidget->hide();
        controlPanel->hide();
        controlPanelVertical->PlaylistButton->hide();
        controlPanelVertical->PlaylistOpenButton->show();
        controlPanelVertical->hide();
    });
    connect(controlPanelVertical->PlaylistOpenButton, &QPushButton::clicked, [this]() {
        listpanel->hide();
        videoWidget->show();
        controlPanel->show();
        controlPanelVertical->PlaylistButton->show();
        controlPanelVertical->PlaylistOpenButton->hide();
    });
    connect(controlPanel->FullscreenButton, &QPushButton::clicked, [this](){
        controlPanel->FullscreenButton->hide();
        controlPanel->FullscreenExitButton->show();
        this->showFullScreen();
    });
    connect(controlPanel->FullscreenExitButton, &QPushButton::clicked, [this](){
        controlPanel->FullscreenButton->show();
        controlPanel->FullscreenExitButton->hide();
        this->showNormal();
    });
    connect(listpanel->ui->CloseButton, &QPushButton::clicked, [this](){
        videoWidget->show();
        controlPanel->show();
    });
    connect(listpanel->ui->pushButton, &QPushButton::clicked, [this](){
        if (lang_mode == 0){
            // translation
            qApp->removeTranslator(&translator);
            if(translator.load(":/translations/zh.qm")){
                qApp->installTranslator(&translator);
            };
            listpanel->ui->retranslateUi(this);
            listpanel->ui->retranslateUi(listpanel->ui->pushButton);
            lang_mode = 1;
        }
        else if (lang_mode == 1){
            // translation
            qApp->removeTranslator(&translator);
            if(translator.load(":/translations/en.qm")){
                qApp->installTranslator(&translator);
            };
            listpanel->ui->retranslateUi(this);
            listpanel->ui->retranslateUi(listpanel->ui->pushButton);
            lang_mode = 0;
        }
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
            if(screen_mode == 1){
                listpanel->setMaximumWidth(300);
                screen_mode = 0;
                listpanel->ui->CloseButton->show();
                // Larger than 900, show controlPanel, hide controlPanelVertical, show listPanel
                videoWidget->show();
                controlPanel->show();
                controlPanel->BackwardButton->show();
                controlPanel->ForwardButton->show();
                if (this->isFullScreen()){
                    controlPanel->FullscreenButton->hide();
                }else{
                    controlPanel->FullscreenButton->show();
                }

                controlPanel->SkipPrevButton->show();
                controlPanel->SkipNextButton->show();
                controlPanel->PlaylistOpenButton->show();
                // Check if the volume is 0 and show VolumeOffButton accordingly
                if (player->volume() == 0) {
                    controlPanel->VolumeOffButton->show();
                } else {
                    controlPanel->VolumeWidget->show();
                }
                if (player->state() == QMediaPlayer::PlayingState){
                    controlPanel->PauseButton->show();

                }else{
                    controlPanel->PlayButton->show();
                }
                listpanel->show();
                controlPanelVertical->hide();
                screen_mode = 0;
            }

        } else if(width <= 900) {
            if(screen_mode == 0){
                listpanel->setMaximumWidth(QWIDGETSIZE_MAX);
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
                screen_mode = 1;
                controlPanelVertical->PlaylistOpenButton->hide();
                controlPanelVertical->PlaylistButton->show();

            }
        }
    }

    QWidget::resizeEvent(event);
}
