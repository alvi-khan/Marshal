#ifndef FILESHAREDIALOG_H
#define FILESHAREDIALOG_H

#include <QDialog>

namespace Ui {
class FileShareDialog;
}

/**
 * @brief The FileShareDialog class handles the dialog shown to pick a user to share with
 */
class FileShareDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileShareDialog(QWidget *parent = nullptr);
    ~FileShareDialog();
    QString username;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_username_textChanged(const QString &arg1);

private:
    Ui::FileShareDialog *ui;
    void init();
    void closeEvent(QCloseEvent *event);
};

#endif // FILESHAREDIALOG_H
