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

/**
 * @brief The CalendarEvent class contains methods and properties for event widgets for calendar blocks
 */
class CalendarEvent : public QFrame
{
    Q_OBJECT

public:
    explicit CalendarEvent(QWidget *parent, QDate eventDate, QString eventName);
    ~CalendarEvent();
    void openEvent();
    Calendar *calendar;
    QString parentPath;
    QString selfPath;
    QString eventName;
    QDate eventDate;
    QTime reminderTime;
    void saveToDisk();
    QString getEventFilePath();
    void setEventName(QString eventName);
    QString getEventName();
    void addToCalendar();
    void deleteEvent(CalendarEvent *event);
    void modifyEvent(CalendarEvent *event);
    void updateEvent();
    void retrieveReminderTime();
public slots:
    void onCustomContextMenu(const QPoint &);
protected:
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
    Ui::CalendarEvent *ui;
};

#endif // CALENDAREVENT_H
