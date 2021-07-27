#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <QLineEdit>
#include <QObject>

class CalendarEvent : public QLineEdit
{
    Q_OBJECT
public:
    CalendarEvent(QString text, QString parentPath);
    void openEvent();
    QString parentPath;
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
};

#endif // CALENDAREVENT_H
