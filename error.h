#ifndef ERROR_H
#define ERROR_H

#include <QDialog>

namespace Ui {
class Error;
}

/**
 * @brief The Error class handles the error dialog.
 */
class Error : public QDialog
{
    Q_OBJECT

public:
    explicit Error(QWidget *parent = nullptr, QString errorMessage = "An error has occurred.");
    ~Error();
    static void showError();

    void closeEvent(QCloseEvent *event);
    bool errorAccepted() const;

private slots:

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::Error *ui;
    bool errorAcceptanceStatus;
};

#endif // ERROR_H
