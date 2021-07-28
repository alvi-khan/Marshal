#include "calendarevent.h"
#include "displaymanager.h"

CalendarEvent::CalendarEvent(QString text, QString parentPath) : parentPath(parentPath)
{
    parentPath.truncate(parentPath.lastIndexOf(QChar('/')));
    this->parentPath = parentPath;

    this->setText(text);
    this->setFrame(QFrame::NoFrame);
    this->setReadOnly(true);
    this->setStyleSheet("background-color: rgb(42, 202, 124); "  // green
                            "border: 1px solid rgb(47, 52, 55); "   // same as page (hidden border)
                            "border-radius: 5px;");
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
