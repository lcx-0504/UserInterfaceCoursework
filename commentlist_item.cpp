#include "commentlist_item.h"
#include "ui_commentlist_item.h"
#include "commentdialog.h"

CommentListItem::CommentListItem(int commentId, const QString& username, const QString& content, QWidget *parent)
    : QWidget(parent), ui(new Ui::CommentListItem), commentId(commentId)
{
    ui->setupUi(this);
    setUsername(username);
    setContent(content);

    // Bind button event
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
    emit editRequested(commentId, getUsername(), getContent());
}


void CommentListItem::onDeleteButtonClicked() {
    emit deleteRequested(commentId);
}

