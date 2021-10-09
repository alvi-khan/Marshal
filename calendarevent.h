#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include "calendar.h"

#include <QLineEdit>
#include <QObject>
#include <QDate>

class CalendarEvent : public QLineEdit
{
    Q_OBJECT
public:
    CalendarEvent(Calendar *calendar, QDate date, QString eventName = "");
    void openEvent();
    Calendar *calendar;
    QString parentPath;
    QDate eventDate;
    void saveToDisk();
    QString getEventFilePath();
    void setEventName(QString eventName);
    void addToCalendar();
    void deleteEvent(CalendarEvent *event);
public slots:
    void onCustomContextMenu(const QPoint &);
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
};

#endif // CALENDAREVENT_H
