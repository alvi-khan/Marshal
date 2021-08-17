#include "calendarevent.h"
#include "displaymanager.h"
#include "filemanager.h"

#include <QDir>

CalendarEvent::CalendarEvent(Calendar *calendar, QDate eventDate, QString eventName)
{
    this->calendar = calendar;
    parentPath = calendar->selfPath;
    parentPath.truncate(parentPath.lastIndexOf(QChar('/')));
    this->eventDate = eventDate;
    this->setText(eventName);

    this->setFrame(QFrame::NoFrame);
    this->setReadOnly(true);
    this->setStyleSheet("background-color: rgb(42, 202, 124); "  // green
                            "border: 1px solid rgb(47, 52, 55); "   // same as page (hidden border)
                            "border-radius: 5px;");
}

void CalendarEvent::setEventName(QString eventName)
{
    this->setText(eventName);
}

void CalendarEvent::saveToDisk()
{
    QString filePath = "/" + this->text();
    QDir dir(parentPath + filePath);
    dir.mkpath(dir.path());
    FileManager::readFromFile(dir.path() + "/files.mar");
    FileManager::appendToFile(parentPath + "/files.cal", eventDate.toString("dd/MM/yyyy\n"));
    FileManager::appendToFile(parentPath + "/files.cal", filePath + "/files.mar\n");
}

QString CalendarEvent::getEventFilePath()
{
    QString filePath = "/" + this->text() + "/files.mar";
    return filePath;
}

void CalendarEvent::mousePressEvent(QMouseEvent *event)
{
    CalendarEvent::openEvent();
    // perform default functionality
    QLineEdit::mousePressEvent(event);
}

void CalendarEvent::openEvent()
{
    QString filePath = parentPath + "/" + this->text();
    DisplayManager::openFileFromPath(filePath, this->text());
}

void CalendarEvent::addToCalendar()
{
    calendar->addToDateCell(this->eventDate, this);
    calendar->heightReset();
    saveToDisk();
    calendar->addToEventList(eventDate, getEventFilePath());
}

