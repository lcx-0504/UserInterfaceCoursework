#include "listpanel.h"
#include "ui_listpanel.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QTime>
#include <QInputDialog>
#include "playlist_item.h"
#include "commentlist_item.h"
#include "datamanager.h"
#include "commentdialog.h"

void ListPanel::loadVideos(const QString& folderPath, QListWidget* listWidget) {
    QDir dir(folderPath);
    QStringList filters;
#if defined(_WIN32)
    filters << "*.wmv";
#else
    filters << "*.mp4" << "*.mov";
#endif
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);

    listWidget->clear(); // Clear list

    for (const QString& fileName : dir.entryList()) {
        QString filePath = dir.absoluteFilePath(fileName);

        // Set the title and default duration
        QString title = QFileInfo(fileName).fileName(); // Use the file name as the title

        // Load thumbnail
        QString thumbPath = filePath.left(filePath.lastIndexOf('.')) + ".png";
        QPixmap thumbnail;
        if (QFile::exists(thumbPath)) {
            thumbnail.load(thumbPath);
        } else {
            thumbnail = QPixmap(":/images/default_thumbnail.png"); // Use the default thumbnail
        }

        // Loading video duration
        QString duration = "--:--"; // Default duration
        QMediaPlayer mediaPlayer;
        mediaPlayer.setMedia(QUrl::fromLocalFile(filePath));
        QEventLoop loop;
        QObject::connect(&mediaPlayer, &QMediaPlayer::durationChanged, [&mediaPlayer, &duration, &loop](qint64 mediaDuration) {
            duration = QTime(0, 0).addMSecs(mediaDuration).toString("mm:ss");
            loop.quit();
        });

        // Wait for the metadata to load
        mediaPlayer.play();
        loop.exec();

        // Create a custom list item
        QListWidgetItem* item = new QListWidgetItem(listWidget);
        PlayListItem* customItem = new PlayListItem;
        customItem->updateInfo(title, thumbnail, duration, filePath); // Incoming video path when updated

        item->setSizeHint(customItem->sizeHint());
        listWidget->setItemWidget(item, customItem);
    }
}

// Insert a zero-width space into the path to force a newline
QString ListPanel::insertZeroWidthSpace(const QString& text) {
    QString result;
    for (int i = 0; i < text.length(); ++i) {
        result.append(text[i]);
        result.append("\u200B");
    }
    return result;
}

void ListPanel::openFolder(const QString& folderPath) {
    if (folderPath.isEmpty() || !QDir(folderPath).exists()) {
        QMessageBox::warning(this,QObject::tr("Invalid Path"), QObject::tr("The specified folder does not exist."));
        return;
    }

    // Load video

    // Update path display
    ui->CurrentPathLabel->setText(insertZeroWidthSpace(folderPath));
    ui->CurrentPathLabel->setToolTip(folderPath);

    // Initializes or updates the DataManager
    if (dataManager) {
        delete dataManager;
    }
    dataManager = new DataManager(folderPath);
    updateButtonStates(false);
    loadVideos(folderPath, ui->PlaylistListWidget);

    // Disable button because no specific file has been selected

}

// Set the status of the Like/favorite/New comment button in batches
void ListPanel::updateButtonStates(bool enabled) {
    ui->FavouriteButton->setEnabled(enabled);
    ui->FavouriteOnButton->setEnabled(enabled);
    ui->CollectButton->setEnabled(enabled);
    ui->CollectOnButton->setEnabled(enabled);
    ui->NewCommentButton->setEnabled(enabled);
    controlPanel->BackwardButton->setEnabled(enabled);
    controlPanel->ForwardButton->setEnabled(enabled);
    controlPanel->PlayButton->setEnabled(enabled);
    controlPanel->SkipPrevButton->setEnabled(enabled);
    controlPanel->SkipNextButton->setEnabled(enabled);
    controlPanel->PauseButton->setEnabled(enabled);
    controlPanelVertical->SkipPrevButton->setEnabled(enabled);
    controlPanelVertical->SkipNextButton->setEnabled(enabled);
    controlPanelVertical->FavouriteButton->setEnabled(enabled);
    controlPanelVertical->FavouriteOnButton->setEnabled(enabled);
    controlPanelVertical->CollectButton->setEnabled(enabled);
    controlPanelVertical->CollectOnButton->setEnabled(enabled);
    if(currentPlayMode ==  PlayMode::RandomLoop){
        controlPanelVertical->SkipPrevButton->setEnabled(false);
        controlPanel->SkipPrevButton->setEnabled(false);
    }
}

ListPanel::ListPanel(QWidget *parent, QHBoxLayout *top, ThePlayer *player, ControlPanel *controlPanel, ControlPanelVertical *controlPanelVertical) :
    QWidget(parent),
    ui(new Ui::ListPanel),
    topLayout(top),
    player(player),
    controlPanel(controlPanel),
    controlPanelVertical(controlPanelVertical),
    currentVideoPath(""),
    dataManager(nullptr)
{
    ui->setupUi(this);

    // this->setMaximumWidth(800);
    ui->FavouriteOnButton->hide();
    ui->CollectOnButton->hide();
    ui->CommentOnButton->hide();
    ui->CommentWidget->hide();
    ui->PlaylistButton->hide();
    controlPanelVertical->PlaylistButton2->hide();

    // Set Now Playing to empty
    PlayListItem* blankListItem = new PlayListItem();
    ui->PlayingItemWidget->layout()->addWidget(blankListItem);
    blankListItem->updateInfo(QObject::tr(""), QPixmap(":/images/default_thumbnail.png"), 0,QObject::tr("No Path")); // 更新时传入视频路径

    // Click the Open folder button
    QObject::connect(ui->OpenFolderButton, &QPushButton::clicked, [this]() {
        QString folderPath = QFileDialog::getExistingDirectory(this, QObject::tr("Select Video Folder"));
        openFolder(folderPath);
    });

    // Select the list item, play the corresponding video, and load the user data
    QObject::connect(ui->PlaylistListWidget, &QListWidget::itemSelectionChanged, this, &ListPanel::playSelectedVideo);


    // The Like and favorite buttons
    QObject::connect(ui->FavouriteButton, &QPushButton::clicked, this, &ListPanel::onLikeClicked);
    QObject::connect(ui->FavouriteOnButton, &QPushButton::clicked, this, &ListPanel::onLikeClicked);
    QObject::connect(ui->CollectButton, &QPushButton::clicked, this, &ListPanel::onCollectClicked);
    QObject::connect(ui->CollectOnButton, &QPushButton::clicked, this, &ListPanel::onCollectClicked);
    QObject::connect(controlPanelVertical->FavouriteButton, &QPushButton::clicked, this, &ListPanel::onLikeClicked);
    QObject::connect(controlPanelVertical->FavouriteOnButton, &QPushButton::clicked, this, &ListPanel::onLikeClicked);
    QObject::connect(controlPanelVertical->CollectButton, &QPushButton::clicked, this, &ListPanel::onCollectClicked);
    QObject::connect(controlPanelVertical->CollectOnButton, &QPushButton::clicked, this, &ListPanel::onCollectClicked);

    // Add comment
    QObject::connect(ui->NewCommentButton, &QPushButton::clicked, this, &ListPanel::onNewComment);

    // Toggle list
    QObject::connect(ui->CommentButton, &QPushButton::clicked, [=]() {
        ui->CommentButton->hide();
        controlPanelVertical->CommentButton->hide();
        ui->CommentOnButton->show();
        controlPanelVertical->CommentOnButton->show();
        ui->PlaylistButton->show();
        controlPanelVertical->PlaylistButton2->show();
        ui->PlaylistOnButton->hide();
        controlPanelVertical->PlaylistOnButton->hide();
        ui->CommentWidget->show();
        ui->PlaylistWidget->hide();
    });
    QObject::connect(controlPanelVertical->CommentButton, &QPushButton::clicked, [=]() {
        ui->CommentButton->hide();
        controlPanelVertical->CommentButton->hide();
        ui->CommentOnButton->show();
        controlPanelVertical->CommentOnButton->show();
        ui->PlaylistButton->show();
        controlPanelVertical->PlaylistButton2->show();
        ui->PlaylistOnButton->hide();
        controlPanelVertical->PlaylistOnButton->hide();
        ui->CommentWidget->show();
        ui->PlaylistWidget->hide();
        show();

    });
    QObject::connect(ui->PlaylistButton, &QPushButton::clicked, [=]() {

        ui->CommentButton->show();
        controlPanelVertical->CommentButton->show();
        ui->CommentOnButton->hide();
        controlPanelVertical->CommentOnButton->hide();
        ui->PlaylistButton->hide();
        controlPanelVertical->PlaylistButton2->hide();
        ui->PlaylistOnButton->show();
        controlPanelVertical->PlaylistOnButton->show();
        ui->CommentWidget->hide();
        ui->PlaylistWidget->show();
    });
    QObject::connect(controlPanelVertical->PlaylistButton2, &QPushButton::clicked, [=]() {
        show();

        ui->CommentButton->show();
        controlPanelVertical->CommentButton->show();
        ui->CommentOnButton->hide();
        controlPanelVertical->CommentOnButton->hide();
        ui->PlaylistButton->hide();
        controlPanelVertical->PlaylistButton2->hide();
        ui->PlaylistOnButton->show();
        controlPanelVertical->PlaylistOnButton->show();
        ui->CommentWidget->hide();
        ui->PlaylistWidget->show();
    });

    // Close listPanel
    QObject::connect(ui->CloseButton, &QPushButton::clicked, [=]() {
        if(controlPanel->isHidden()){
            controlPanelVertical->show();
        }else{
            controlPanel->PlaylistOpenButton->hide();
            controlPanel->PlaylistButton->show();
        }
        controlPanelVertical->PlaylistOpenButton->hide();
        controlPanelVertical->PlaylistButton->show();
        this->hide();
    });

    // play mode
    currentPlayMode = PlayMode::ListLoop;
    QObject::connect(ui->PlayModeListLoopButton, &QPushButton::clicked, this, &ListPanel::onPlayModeButtonClicked);
    QObject::connect(ui->PlayModeSingleLoopButton, &QPushButton::clicked, this, &ListPanel::onPlayModeButtonClicked);
    QObject::connect(ui->PlayModeRandomLoopButton, &QPushButton::clicked, this, &ListPanel::onPlayModeButtonClicked);
    QObject::connect(ui->PlayModeNoLoopButton, &QPushButton::clicked, this, &ListPanel::onPlayModeButtonClicked);

    connect(player, &ThePlayer::playbackFinished, this, &ListPanel::handlePlaybackFinished);

    updatePlayModeUI();

    // Bind the SkipNext and Prev buttons in control
    QObject::connect(controlPanel->SkipPrevButton, &QPushButton::clicked, this, &ListPanel::playPreviousVideo);
    QObject::connect(controlPanel->SkipNextButton, &QPushButton::clicked, this, &ListPanel::playNextVideo);
    // Bind the SkipNext and Prev buttons in the controlveitical
    QObject::connect(controlPanelVertical->SkipPrevButton, &QPushButton::clicked, this, &ListPanel::playPreviousVideo);
    QObject::connect(controlPanelVertical->SkipNextButton, &QPushButton::clicked, this, &ListPanel::playNextVideo);


    // The initialize button is disabled
    updateButtonStates(false);
}

void ListPanel::playSelectedVideo() {
    if (player) {
        player->stopManually(); //Make sure the player enters manual stop mode
    }

    QListWidgetItem* item = ui->PlaylistListWidget->currentItem();
    if (!item) return;

    PlayListItem* customItem = qobject_cast<PlayListItem*>(ui->PlaylistListWidget->itemWidget(item));
    if (!customItem || !player) return;

    currentVideoPath = customItem->getVideoPath();
    player->playVideo(currentVideoPath);

    // Update likes and favorites status
    updateLikeButton(dataManager->getLikeStatus(currentVideoPath));
    updateCollectButton(dataManager->getCollectStatus(currentVideoPath));

    // Refresh the comment list
    refreshComments();

    // Enable button
    updateButtonStates(true);

    // Update the Playing area
    if (ui->PlayingItemWidget) {

        while (QLayoutItem* child = ui->PlayingItemWidget->layout()->takeAt(0)) {
            if (child->widget()) {
                delete child->widget();
            }
            delete child;
        }
        PlayListItem* playingItem = new PlayListItem;
        playingItem->updateInfo(
            customItem->getTitle(),
            customItem->getThumbnail(),
            customItem->getDuration(),
            customItem->getVideoPath()
        );
        ui->PlayingItemWidget->layout()->addWidget(playingItem);
    }
}

void ListPanel::refreshComments() {
    ui->CommentListWidget->clear();

    // Disable the button when the video path is empty
    if (currentVideoPath.isEmpty()) {
        updateButtonStates(false);
        return;
    }

    QList<QVariantMap> comments = dataManager->getComments(currentVideoPath);
    for (const QVariantMap &commentData : comments) {
        int id = commentData["id"].toInt();
        QString username = commentData["username"].toString();
        QString content = commentData["content"].toString();

        CommentListItem *commentItem = new CommentListItem(id, username, content, this);

        QObject::connect(commentItem, &CommentListItem::editRequested, this, &ListPanel::onEditCommentRequested);
        connect(commentItem, &CommentListItem::deleteRequested, this, &ListPanel::onDeleteComment);

        QListWidgetItem *listItem = new QListWidgetItem(ui->CommentListWidget);
        listItem->setSizeHint(commentItem->sizeHint());
        ui->CommentListWidget->setItemWidget(listItem, commentItem);
    }
}

void ListPanel::onNewComment() {
    // Record the current playback status and pause
    bool wasPlaying = (player->state() == QMediaPlayer::PlayingState);
    player->pause();

    // Create a comment dialog
    CommentDialog *dialog = new CommentDialog(this);
    dialog->setDialogMode(false); // New mode
    dialog->setWindowModality(Qt::ApplicationModal);

    // Displays the dialog box and processes the results
    if (dialog->exec() == QDialog::Accepted) {
        QString username = dialog->getUsername();
        QString content = dialog->getCommentContent();

        if (!username.isEmpty() && !content.isEmpty()) {
            dataManager->addComment(currentVideoPath, username, content);
            refreshComments();
        }
    }

    // recovering playing state
    if (wasPlaying) {
        player->play();
    }

    delete dialog;
}


// Processing edit requests
void ListPanel::onEditCommentRequested(int commentId, const QString &username, const QString &content) {
    bool wasPlaying = (player->state() == QMediaPlayer::PlayingState);
    player->pause();

    // Create a comment dialog
    CommentDialog *dialog = new CommentDialog(this);
    dialog->setDialogMode(true); // edit mode
    dialog->setCommentData(username, content);
    dialog->setWindowModality(Qt::ApplicationModal);

    // Displays the dialog box and processes the results
    if (dialog->exec() == QDialog::Accepted) {
        QString newUsername = dialog->getUsername();
        QString newContent = dialog->getCommentContent();

        if (!newUsername.isEmpty() && !newContent.isEmpty()) {
            dataManager->editComment(currentVideoPath, commentId, newUsername, newContent);
            refreshComments();
        }
    }
    if (wasPlaying) {
        player->play();
    }

    delete dialog;
}

void ListPanel::onDeleteComment(int commentId) {
    dataManager->deleteComment(currentVideoPath, commentId);
    refreshComments();
}

void ListPanel::onLikeClicked() {
    bool currentStatus = dataManager->getLikeStatus(currentVideoPath);
    dataManager->setLikeStatus(currentVideoPath, !currentStatus);
    updateLikeButton(!currentStatus);
}

void ListPanel::updateLikeButton(bool liked) {
    ui->FavouriteButton->setVisible(!liked);
    ui->FavouriteOnButton->setVisible(liked);
    controlPanelVertical->FavouriteButton->setVisible(!liked);
    controlPanelVertical->FavouriteOnButton->setVisible(liked);
}

void ListPanel::onCollectClicked() {
    bool currentStatus = dataManager->getCollectStatus(currentVideoPath);
    dataManager->setCollectStatus(currentVideoPath, !currentStatus);
    updateCollectButton(!currentStatus);
}

void ListPanel::updateCollectButton(bool collected) {
    ui->CollectButton->setVisible(!collected);
    ui->CollectOnButton->setVisible(collected);
    controlPanelVertical->CollectButton->setVisible(!collected);
    controlPanelVertical->CollectOnButton->setVisible(collected);
}

// play mode
void ListPanel::onPlayModeButtonClicked() {
    // switch the next playmode
    switch (currentPlayMode) {
        case PlayMode::ListLoop:
            currentPlayMode = PlayMode::SingleLoop;
            break;
        case PlayMode::SingleLoop:
            currentPlayMode = PlayMode::RandomLoop;
            break;
        case PlayMode::RandomLoop:
            currentPlayMode = PlayMode::NoLoop;
            break;
        case PlayMode::NoLoop:
            currentPlayMode = PlayMode::ListLoop;
            break;
    }

    // update
    updatePlayModeUI();
}

void ListPanel::updatePlayModeUI() {
    // update button state
    ui->PlayModeListLoopButton->setVisible(currentPlayMode == PlayMode::ListLoop);
    ui->PlayModeSingleLoopButton->setVisible(currentPlayMode == PlayMode::SingleLoop);
    ui->PlayModeRandomLoopButton->setVisible(currentPlayMode == PlayMode::RandomLoop);
    ui->PlayModeNoLoopButton->setVisible(currentPlayMode == PlayMode::NoLoop);

    // Disable the previous button in random mode
    controlPanel->SkipPrevButton->setDisabled(currentPlayMode == PlayMode::RandomLoop);
    controlPanelVertical->SkipPrevButton->setDisabled(currentPlayMode == PlayMode::RandomLoop);

    // Updated the playback mode TAB
    switch (currentPlayMode) {
        case PlayMode::ListLoop:
            ui->PlayModeLabel->setText(QObject::tr("Play Mode: List Loop"));
            break;
        case PlayMode::SingleLoop:
            ui->PlayModeLabel->setText(QObject::tr("Play Mode: Single Loop"));
            break;
        case PlayMode::RandomLoop:
            ui->PlayModeLabel->setText(QObject::tr("Play Mode: Random Loop"));
            break;
        case PlayMode::NoLoop:
            ui->PlayModeLabel->setText(QObject::tr("Play Mode: No Loop"));
            break;
    }
}

void ListPanel::handlePlaybackFinished() {
    if (currentPlayMode == PlayMode::NoLoop) {
        // No operation is performed after the playback is complete
        return;
    }

    int currentIndex = ui->PlaylistListWidget->currentRow();
    int totalItems = ui->PlaylistListWidget->count();

    // Select the next item to play according to the playback mode
    if (currentPlayMode == PlayMode::ListLoop) {
        currentIndex = (currentIndex + 1) % totalItems;
    } else if (currentPlayMode == PlayMode::SingleLoop) {
    } else if (currentPlayMode == PlayMode::RandomLoop) {
        int newIndex;
        do {
            newIndex = qrand() % totalItems; // randomly choose
        } while (newIndex == currentIndex && totalItems > 1); // Make sure the new index is different from the current index
        currentIndex = newIndex;
    }

    // Sets the current selection
    ui->PlaylistListWidget->setCurrentRow(currentIndex);

    // Call wrapped playback logic
     playSelectedVideo();
}

void ListPanel::playNextVideo() {
    int currentIndex = ui->PlaylistListWidget->currentRow();
    int totalItems = ui->PlaylistListWidget->count();

    if (currentPlayMode == PlayMode::RandomLoop) {
        int nextIndex = qrand() % totalItems;
        while (nextIndex == currentIndex) {
            nextIndex = qrand() % totalItems; // Make sure to randomize to different videos
        }
        currentIndex = nextIndex;
    } else {
        currentIndex = (currentIndex + 1) % totalItems;
    }

    ui->PlaylistListWidget->setCurrentRow(currentIndex);
    playSelectedVideo();
}

void ListPanel::playPreviousVideo() {
    if (currentPlayMode == PlayMode::RandomLoop) {
        return; // Disable the Previous button in random mode
    }

    int currentIndex = ui->PlaylistListWidget->currentRow();
    int totalItems = ui->PlaylistListWidget->count();
    currentIndex = (currentIndex - 1 + totalItems) % totalItems;

    ui->PlaylistListWidget->setCurrentRow(currentIndex);
    playSelectedVideo();
}

ListPanel::~ListPanel() {
    delete ui;
    if (dataManager) {
        delete dataManager;
    }
}
