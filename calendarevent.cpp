#include "calendarevent.h"
#include "displaymanager.h"
#include "filemanager.h"
#include "newevent.h"

#include <QDir>

CalendarEvent::CalendarEvent(QString parentPath, QDate eventDate, QString eventName)
{
    parentPath.truncate(parentPath.lastIndexOf(QChar('/')));
    this->parentPath = parentPath;
    this->eventDate = eventDate;

    if (eventName == "")    eventName = getNewEventName();
    this->setText(eventName);

    this->setFrame(QFrame::NoFrame);
    this->setReadOnly(true);
    this->setStyleSheet("background-color: rgb(42, 202, 124); "  // green
                            "border: 1px solid rgb(47, 52, 55); "   // same as page (hidden border)
                            "border-radius: 5px;");
}

QString CalendarEvent::getNewEventName()
{
    QString eventName = "";
    NewEvent *newEvent = new NewEvent();
    newEvent->exec();

    if (newEvent->accepted && newEvent->fileName != "") eventName = newEvent->fileName;

    delete newEvent;
    return eventName;
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
