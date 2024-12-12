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

    void setDialogMode(bool isEditMode); // 设置对话框模式，true 为编辑模式，false 为新建模式
    void setCommentData(const QString& username, const QString& content); // 设置初始评论数据（用于编辑模式）

    QString getUsername() const;       // 获取输入的用户名
    QString getCommentContent() const; // 获取输入的评论内容

signals:
    void dialogAccepted(const QString& username, const QString& content); // 确定时的信号

private slots:
    void onOKButtonClicked();
    void onCancelButtonClicked();

private:
    Ui::CommentDialog *ui;
    bool editMode; // 当前是否为编辑模式
};

#endif // COMMENTDIALOG_H
