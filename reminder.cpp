#include "reminder.h"
#include "ui_reminder.h"
#include "displaymanager.h"
#include "mainwindow.h"
#include "reminderscontainer.h"
#include "filemanager.h"

#include <QTimer>

Reminder::Reminder(QWidget *parent, QString eventPath, QDateTime reminderTime) :
    QWidget(parent),
    ui(new Ui::Reminder)
{
    ui->setupUi(this);
    timer = new QTimer();
    this->ui->eventName->setText(eventPath.section("/", -2, -2));
    this->ui->date->setText(reminderTime.date().toString("dd/MM/yyyy"));
    this->ui->time->setText(reminderTime.time().toString("hh:mm"));
    Reminder::eventPath = eventPath;
    Reminder::reminderTime = reminderTime;
    expired = false;
    setTimer();
}

Reminder::~Reminder()
{
    delete ui;
}

/**
 * @brief Reminder::expire sets reminder styling to expired reminder style
 */
void Reminder::expire()
{
    expired = true;
    RemindersContainer::updateExpiredReminderCount(1);
    setStyleSheet("QFrame#container{\
                    border: 1.5px solid #772C2C;\
                    background-color: #50772C2C;\
                   }\
                   QFrame#container:hover{background-color: #90732434}");
}

/**
 * @brief Reminder::unexpire sets reminder styling to unexpired reminder style
 */
void Reminder::unexpire()
{
   expired = false;
   RemindersContainer::updateExpiredReminderCount(-1);
   setStyleSheet("QFrame#container{\
                   border: 1.5px solid rgb(70, 70, 70);\
                   background-color: #B0232323;\
                  }\
                  QFrame#container:hover{background-color: #903E3E3E}");
}

/**
 * @brief Reminder::setTimer sets a reminder
 */
void Reminder::setTimer()
{
    int millisecondsDiff = QDateTime::currentDateTime().msecsTo(reminderTime);
    if (millisecondsDiff < 0)   expire();
    else
    {
        connect(timer, &QTimer::timeout, [=]{expire();});
        timer->setSingleShot(true);
        timer->start(millisecondsDiff);
    }
}

/**
 * @brief Reminder::mouseReleaseEvent opens page for reminder's event
 * @param event
 */
void Reminder::mouseReleaseEvent(QMouseEvent *event)
{
    MainWindow::window->revealMainPage();
    DisplayManager::openFileFromPath(eventPath.section("/", 0, -2), this->ui->eventName->text());
    if (expired)    RemindersContainer::removeReminder(this);   // expired reminders are deleted when opened
    RemindersContainer::hideContainer();
    QWidget::mouseReleaseEvent(event);
}

void Reminder::setEventPath(QString eventPath)
{
    // update reminder tracker
    QString remindersStorage = QCoreApplication::applicationDirPath() + "/reminders.dat";
    FileManager::updateFileTracker(remindersStorage, Reminder::eventPath, eventPath);

    Reminder::eventPath = eventPath;
    this->ui->eventName->setText(eventPath.section("/", -2, -2));
}

void Reminder::setEventTime(QDateTime dateTime)
{
    // update reminder tracker
    QString remindersStorage = QCoreApplication::applicationDirPath() + "/reminders.dat";
    QString data = FileManager::readFromFile(remindersStorage);
    data.replace(reminderTime.toString() + "\n" + eventPath, dateTime.toString() + "\n" + eventPath);
    FileManager::writeToFile(remindersStorage, data);

    reminderTime = dateTime;
    this->ui->date->setText(reminderTime.date().toString("dd/MM/yyyy"));
    this->ui->time->setText(reminderTime.time().toString("hh:mm"));
    if (expired && dateTime > QDateTime::currentDateTime()) unexpire();

    if (timer)  timer->stop();
    setTimer();
}
