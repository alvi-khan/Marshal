#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include "calendar.h"

#include <QLineEdit>
#include <QObject>
#include <QDate>
#include <QFrame>

namespace Ui {
class CalendarEvent;
}

class CalendarEvent : public QFrame
{
    Q_OBJECT

public:
    explicit CalendarEvent(QWidget *parent, QDate eventDate, QString eventName);
    ~CalendarEvent();
    void openEvent();
    Calendar *calendar;
    QString parentPath;
    QDate eventDate;
    void saveToDisk();
    QString getEventFilePath();
    void setEventName(QString eventName);
    QString getEventName();
    void addToCalendar();
    void deleteEvent(CalendarEvent *event);
public slots:
    void onCustomContextMenu(const QPoint &);
protected:
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
    Ui::CalendarEvent *ui;
    void retrieveReminderTime();
};

#endif // CALENDAREVENT_H
