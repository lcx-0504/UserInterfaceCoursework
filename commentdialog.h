#ifndef COMMENTDIALOG_H
#define COMMENTDIALOG_H

#include <QDialog>

namespace Ui {
class CommentDialog;
}

class CommentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommentDialog(QWidget *parent = nullptr);
    ~CommentDialog();

    void setDialogMode(bool isEditMode); // Set dialog mode, true to edit mode, false to create mode
    void setCommentData(const QString& username, const QString& content); // Set initial comment data (for edit mode)

    QString getUsername() const;       // Gets the entered user name
    QString getCommentContent() const; // Get the input comment content

signals:
    void dialogAccepted(const QString& username, const QString& content); // The signal of confirmation

private slots:
    void onOKButtonClicked();
    void onCancelButtonClicked();

private:
    Ui::CommentDialog *ui;
    bool editMode; // Whether the current mode is editing
};

#endif // COMMENTDIALOG_H
