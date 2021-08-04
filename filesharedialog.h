#ifndef FILESHAREDIALOG_H
#define FILESHAREDIALOG_H

#include <QDialog>

namespace Ui {
class FileShareDialog;
}

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

private:
    Ui::FileShareDialog *ui;
    void init();
};

#endif // FILESHAREDIALOG_H
