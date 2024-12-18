#include "commentdialog.h"
#include "ui_commentdialog.h"
#include <QMessageBox>

CommentDialog::CommentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommentDialog),
    editMode(false) // Default new mode
{
    ui->setupUi(this);

    // Set the window flag and remove the question mark button from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Bind button event
    connect(ui->OKButton, &QPushButton::clicked, this, &CommentDialog::onOKButtonClicked);
    connect(ui->CancelButton, &QPushButton::clicked, this, &CommentDialog::onCancelButtonClicked);
}

CommentDialog::~CommentDialog()
{
    delete ui;
}

void CommentDialog::setDialogMode(bool isEditMode) {
    editMode = isEditMode;
    ui->Title->setText(isEditMode ? QObject::tr("Edit Comment") : QObject::tr("New Comment")); // Dynamically set title
}

void CommentDialog::setCommentData(const QString& username, const QString& content) {
    ui->UsenameLineEdit->setText(username);
    ui->ContentTextEdit->setPlainText(content);
}

QString CommentDialog::getUsername() const {
    return ui->UsenameLineEdit->text();
}

QString CommentDialog::getCommentContent() const {
    return ui->ContentTextEdit->toPlainText();
}

void CommentDialog::onOKButtonClicked() {
    QString username = getUsername();
    QString content = getCommentContent();

    if (username.isEmpty() || content.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Invalid Input"), QObject::tr("Username and content cannot be empty."));
        return;
    }

    emit dialogAccepted(username, content);
    accept();
}

void CommentDialog::onCancelButtonClicked() {
    reject();
}
