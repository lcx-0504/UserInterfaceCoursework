#ifndef COMMENTLIST_ITEM_H
#define COMMENTLIST_ITEM_H

#include <QWidget>
#include <QInputDialog>

namespace Ui {
class CommentListItem;
}

class CommentListItem : public QWidget
{
    Q_OBJECT

public:
    explicit CommentListItem(int commentId, const QString& username, const QString& content, QWidget *parent = nullptr);
    ~CommentListItem();

    int getCommentId() const;
    QString getUsername() const;
    QString getContent() const;

    void setUsername(const QString& username);
    void setContent(const QString& content);

signals:
    void editRequested(int commentId, const QString& newUsername, const QString& newContent);
    void deleteRequested(int commentId);

private slots:
    void onEditButtonClicked();
    void onDeleteButtonClicked();

private:
    Ui::CommentListItem *ui;
    int commentId;
};

#endif // COMMENTLIST_ITEM_H
