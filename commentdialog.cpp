#include "commentdialog.h"
#include "ui_commentdialog.h"
#include <QMessageBox>

CommentDialog::CommentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommentDialog),
    editMode(false) // 默认新建模式
{
    ui->setupUi(this);

    // 设置窗口标志，移除标题栏中的问号按钮
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // 绑定按钮事件
    connect(ui->OKButton, &QPushButton::clicked, this, &CommentDialog::onOKButtonClicked);
    connect(ui->CancelButton, &QPushButton::clicked, this, &CommentDialog::onCancelButtonClicked);
}

CommentDialog::~CommentDialog()
{
    delete ui;
}

void CommentDialog::setDialogMode(bool isEditMode) {
    editMode = isEditMode;
    ui->Title->setText(isEditMode ? "Edit Comment" : "New Comment"); // 动态设置标题
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
        QMessageBox::warning(this, "Invalid Input", "Username and content cannot be empty.");
        return;
    }

    emit dialogAccepted(username, content);
    accept();
}

void CommentDialog::onCancelButtonClicked() {
    reject();
}
