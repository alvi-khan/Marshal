#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    void init();
private slots:
    void on_loginBtn_clicked();

    void on_regBtn_clicked();

    void on_cancelBtn_clicked();

    void on_logoutBtn_clicked();

private:
    Ui::LoginDialog *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // LOGINDIALOG_H
