#include "calendarevent.h"
#include "ui_calendarevent.h"
#include "displaymanager.h"
#include "filemanager.h"
#include "reminderscontainer.h"
#include "eventdialog.h"

#include <QDir>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>

#include <QtConcurrent/QtConcurrent>
#include <qtconcurrentrun.h>
#include <QThread>
#include <QtConcurrentRun>

CalendarEvent::CalendarEvent(QWidget *parent, QDate eventDate, QString eventName) :
    QFrame(parent),
    ui(new Ui::CalendarEvent)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    Calendar *calendar = (Calendar *) parent;
    this->calendar = calendar;
    parentPath = calendar->selfPath;
    parentPath.truncate(parentPath.lastIndexOf(QChar('/')));
    selfPath = parentPath + "/" + eventName + "/files.mar";
    this->eventDate = eventDate;
    this->ui->eventName->setText(eventName);
    this->eventName = eventName;

    retrieveReminderTime();

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
}

CalendarEvent::~CalendarEvent()
{
    delete ui;
}

void CalendarEvent::setEventName(QString eventName)
{
    this->ui->eventName->setText(eventName);
    this->eventName = eventName;
}

QString CalendarEvent::getEventName()
{
    return this->ui->eventName->text();
}

void CalendarEvent::saveToDisk()
{
    QString filePath = "/" + this->ui->eventName->text();
    QDir dir(parentPath + filePath);
    dir.mkpath(dir.path());
    FileManager::readFromFile(dir.path() + "/files.mar");
    FileManager::appendToFile(parentPath + "/files.cal", eventDate.toString("dd/MM/yyyy\n"));
    FileManager::appendToFile(parentPath + "/files.cal", filePath + "/files.mar\n");
}

QString CalendarEvent::getEventFilePath()
{
    QString filePath = "/" + this->ui->eventName->text() + "/files.mar";
    return filePath;
}

void CalendarEvent::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
            CalendarEvent::openEvent();
}

void CalendarEvent::openEvent()
{
    QString filePath = parentPath + "/" + this->ui->eventName->text();
    DisplayManager::openFileFromPath(filePath, this->ui->eventName->text());
}

void CalendarEvent::addToCalendar()
{
    retrieveReminderTime();

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

    QAction* modify = new QAction("Modify", this);
    modify->setIcon(QIcon(":/Icons/Resources/Icons/Modify.svg"));

    connect(del, &QAction::triggered, [=] {deleteEvent(this);});
    menu.addAction(del);

    connect(modify, &QAction::triggered, [=] {modifyEvent(this);});
    menu.addAction(modify);

    menu.exec(QCursor::pos());
    del->deleteLater();
}

void CalendarEvent::deleteEvent(CalendarEvent *event)
{
    event->hide();
    FileManager::updateFileTracker(parentPath + "/files.cal", eventDate.toString("dd/MM/yyyy"), "");
    FileManager::updateFileTracker(parentPath + "/files.cal", "/" + this->ui->eventName->text() + "/files.mar", "");
    FileManager::deleteDirectory(parentPath + "/" + this->ui->eventName->text());
    calendar->heightReset();
    RemindersContainer::refreshReminderList();
}

void CalendarEvent::modifyEvent(CalendarEvent *event)
{
    EventDialog *eventDialog = new EventDialog();
    QPoint position = this->pos();
    position = this->mapToGlobal(position);
    eventDialog->displayDialog(this, position);
    connect(eventDialog, &EventDialog::hidden, [=] { updateEvent();});
}

void CalendarEvent::updateEvent()
{
    FileManager::updateFileTracker(parentPath + "/files.cal", "/" + selfPath.section("/", -2, -1), "/" + eventName + "/files.mar");
    QDir dir(selfPath.section("/", 0, -2));
    dir.rename(selfPath.section("/", 0, -2), selfPath.section("/", 0, -3) + "/" + eventName);
    selfPath = selfPath.section("/", 0, -3) + "/" + eventName + "/files.mar";
}

void CalendarEvent::retrieveReminderTime()
{
    QString remindersStorage = QCoreApplication::applicationDirPath() + "/reminders.dat";
    QFile file(remindersStorage);
    file.open(QIODevice::ReadOnly);
    QTextStream data(&file);
    while (!data.atEnd())
    {
        QDateTime dateTime = QDateTime::fromString(data.readLine());
        QString eventPath = data.readLine();
        if (eventPath == (parentPath + "/" + this->ui->eventName->text() + "/files.mar"))
        {
            file.close();
            this->ui->reminderTime->setText(dateTime.toString("hh:mm"));
            this->reminderTime = dateTime.time();
            return;
        }
    }

    file.close();
    this->ui->reminderTime->setText("");
}
