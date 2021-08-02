#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <QLineEdit>
#include <QObject>
#include <QDate>

class CalendarEvent : public QLineEdit
{
    Q_OBJECT
public:
    CalendarEvent(QString parentPath, QDate date, QString eventName = "");
    void openEvent();
    QString parentPath;
    QDate eventDate;
    QString getNewEventName();
    void saveToDisk();
    QString getEventFilePath();
    void setEventName(QString eventName);
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
};

#endif // CALENDAREVENT_H
