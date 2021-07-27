#ifndef NEWEVENT_H
#define NEWEVENT_H

#include <QDialog>

namespace Ui {
class NewEvent;
}

class NewEvent : public QDialog
{
    Q_OBJECT

public:
    explicit NewEvent(QWidget *parent = nullptr);
    ~NewEvent();

    void init();
    bool accepted;
    QString fileName;
private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewEvent *ui;

};

#endif // NEWEVENT_H
