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
    void on_eventName_editingFinished();

private:
    Ui::EventDialog *ui;
    CalendarEvent *event;
};

#endif // EVENTDIALOG_H
