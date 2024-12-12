#include "commentlist_item.h"
#include "ui_commentlist_item.h"
#include "commentdialog.h"

CommentListItem::CommentListItem(int commentId, const QString& username, const QString& content, QWidget *parent)
    : QWidget(parent), ui(new Ui::CommentListItem), commentId(commentId)
{
    ui->setupUi(this);
    setUsername(username);
    setContent(content);

    // 绑定按钮事件
    connect(ui->EditButton, &QPushButton::clicked, this, &CommentListItem::onEditButtonClicked);
    connect(ui->DeleteButton, &QPushButton::clicked, this, &CommentListItem::onDeleteButtonClicked);
}

CommentListItem::~CommentListItem()
{
    delete ui;
}

int CommentListItem::getCommentId() const {
    return commentId;
}

QString CommentListItem::getUsername() const {
    return ui->Username->text();
}

QString CommentListItem::getContent() const {
    return ui->Content->text();
}

void CommentListItem::setUsername(const QString& username) {
    ui->Username->setText(username);
}

void CommentListItem::setContent(const QString& content) {
    ui->Content->setText(content);
}

void CommentListItem::onEditButtonClicked() {
    // 创建 CommentDialog 对象
    CommentDialog *dialog = new CommentDialog(this); // 不设置 parent，作为独立窗口

    // 设置为编辑模式并初始化数据
    dialog->setDialogMode(true);
    dialog->setCommentData(getUsername(), getContent());

    // 设置为模态窗口
    dialog->setWindowModality(Qt::ApplicationModal); // 或 dialog->setModal(true);

    // 弹出对话框并阻塞其他操作
    if (dialog->exec() == QDialog::Accepted) {
        QString newUsername = dialog->getUsername();
        QString newContent = dialog->getCommentContent();

        if (!newUsername.isEmpty() && !newContent.isEmpty()) {
            emit editRequested(commentId, newUsername, newContent);
            setUsername(newUsername); // 更新显示的用户名
            setContent(newContent);   // 更新显示的内容
        }
    }

    // 释放资源
    delete dialog;
}

void CommentListItem::onDeleteButtonClicked() {
    emit deleteRequested(commentId);
}

