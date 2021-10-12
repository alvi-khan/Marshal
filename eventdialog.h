#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QWidget>
#include <QLineEdit>
#include "calendarevent.h"

namespace Ui {
class EventDialog;
}

class EventDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EventDialog(QWidget *parent = nullptr);
    ~EventDialog();
    void displayDialog(CalendarEvent *event, QPoint point);
private slots:
    void closeEvent(QCloseEvent *closeEvent);

    void on_checkBox_toggled(bool checked);

private:
    Ui::EventDialog *ui;
    CalendarEvent *event;
    void setReminder();
signals:
    void hidden();
};

#endif // EVENTDIALOG_H
