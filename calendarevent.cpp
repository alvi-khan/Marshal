#include "calendarevent.h"
#include "displaymanager.h"
#include "filemanager.h"

#include <QDir>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>

CalendarEvent::CalendarEvent(Calendar *calendar, QDate eventDate, QString eventName)
{
    this->calendar = calendar;
    parentPath = calendar->selfPath;
    parentPath.truncate(parentPath.lastIndexOf(QChar('/')));
    this->eventDate = eventDate;
    this->setText(eventName);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    this->setFrame(QFrame::NoFrame);
    this->setReadOnly(true);
    this->setStyleSheet("background-color: rgb(42, 202, 124); "  // green
                            "border: 1px solid rgb(42, 202, 124); "   // same as event (hidden border)
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
    if (event->button() == Qt::LeftButton)
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

void CalendarEvent::onCustomContextMenu(const QPoint &)
{
    QMenu menu;
    menu.setStyleSheet("QMenu{background-color: rgb(46, 46, 46); color: rgb(255, 255, 255); padding: 2px;}\
                        QMenu::item:selected{background-color: #3E3E3E; color: rgb(255, 255, 255); border-radius: 2px;}");

    QAction* del = new QAction("Delete", this);
    del->setIcon(QIcon(":/Toolbar Icons/Resources/Toolbar Icons/Trash (Context Menu).svg"));

    connect(del, &QAction::triggered, [=] {deleteEvent(this);});
    menu.addAction(del);

    menu.exec(QCursor::pos());
    del->deleteLater();
}

void CalendarEvent::deleteEvent(CalendarEvent *event)
{
    event->hide();
    FileManager::updateFileTracker(parentPath + "/files.cal", eventDate.toString("dd/MM/yyyy"), "");
    FileManager::updateFileTracker(parentPath + "/files.cal", "/" + text() + "/files.mar", "");
    FileManager::deleteDirectory(parentPath + "/" + text());
    calendar->heightReset();
}

