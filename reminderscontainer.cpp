#include "reminderscontainer.h"
#include "ui_reminderscontainer.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <filemanager.h>

QList<Reminder *> RemindersContainer::reminders;
RemindersContainer * RemindersContainer::activeContainer;
int RemindersContainer::expiredReminders = 0;

RemindersContainer::RemindersContainer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemindersContainer)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    effect->setXOffset(5);
    effect->setYOffset(5);
    effect->setColor(QColor("#1E1E1E"));
    this->ui->frame->setGraphicsEffect(effect);

    init();
}

RemindersContainer::~RemindersContainer()
{
    delete ui;
}

void RemindersContainer::init()
{
    activeContainer = this;
    foreach(Reminder *reminder, reminders)  // add reminders
    {
        this->ui->mainArea->layout()->addWidget(reminder);
        reminder->show();
    }

    QPoint point = MainWindow::window->ui->remindersButton->mapToGlobal(MainWindow::window->ui->remindersButton->rect().topRight());

    this->move(point.x() + 20, point.y());
    this->show();
    this->adjustSize();
}

void RemindersContainer::hideContainer()
{
    if (activeContainer != nullptr)
    {
        activeContainer->hide();
        activeContainer = nullptr;
    }
}

/**
 * @brief RemindersContainer::addReminder adds a reminder to the visible list
 * @param eventPath
 * @param dateTime
 */
void RemindersContainer::addReminder(QString eventPath, QDateTime dateTime)
{
    Reminder *reminder = new Reminder(MainWindow::window, eventPath, dateTime);
    reminders.append(reminder);
    reminder->hide();
}

/**
 * @brief RemindersContainer::removeReminder deletes reminder from visilble list and tracker
 */
void RemindersContainer::removeReminder(Reminder *reminder)
{
    reminders.removeOne(reminder);
    deleteReminder(reminder->eventPath, reminder->reminderTime);
    if (reminder->reminderTime <= QDateTime::currentDateTime())
        updateExpiredReminderCount(-1);
}

/**
 * @brief RemindersContainer::removeReminder deletes reminder from visilble list and tracker
 */
void RemindersContainer::removeReminder(QString eventPath)
{
    foreach(Reminder *reminder, reminders)
    {
        if (reminder->eventPath == eventPath)
        {
            removeReminder(reminder);
            return;
        }
    }
}

/**
 * @brief RemindersContainer::deleteReminder removes reminder from reminder tracker
 */
void RemindersContainer::deleteReminder(QString eventPath, QDateTime dateTime)
{
    QString remindersStorage = QCoreApplication::applicationDirPath() + "/reminders.dat";
    QString data = FileManager::readFromFile(remindersStorage);
    data.remove(dateTime.toString() + "\n" + eventPath + "\n");
    FileManager::writeToFile(remindersStorage, data);
}

/**
 * @brief RemindersContainer::refreshReminderList cleans up invalid reminders
 */
void RemindersContainer::refreshReminderList()
{
    int expiryCount = 0;
    foreach (Reminder *reminder, reminders)
    {
        if (!QFile::exists(reminder->eventPath))    removeReminder(reminder);
        else if (reminder->reminderTime <= QDateTime::currentDateTime())    expiryCount++;
    }

    updateExpiredReminderCount(-(expiredReminders - expiryCount));
}

/**
 * @brief RemindersContainer::createNewReminder adds reminder to tracker
 * @param eventPath
 * @param dateTime
 */
void RemindersContainer::createNewReminder(QString eventPath, QDateTime dateTime)
{
    QString remindersStorage = QCoreApplication::applicationDirPath() + "/reminders.dat";
    FileManager::appendToFile(remindersStorage, dateTime.toString() + "\n");
    FileManager::appendToFile(remindersStorage, eventPath + "\n");
    addReminder(eventPath, dateTime);
}

void RemindersContainer::retrieveReminders()
{
    QString remindersStorage = QCoreApplication::applicationDirPath() + "/reminders.dat";
    QFile file(remindersStorage);
    file.open(QIODevice::ReadOnly);
    QTextStream data(&file);
    while (!data.atEnd())
    {
        QDateTime dateTime = QDateTime::fromString(data.readLine());
        QString eventPath = data.readLine();
        addReminder(eventPath, dateTime);
    }

    file.close();
}

/**
 * @brief RemindersContainer::updateExpiredReminderCount sets the number of expired reminders
 * @param count is the value to change by (+ or -)
 */
void RemindersContainer::updateExpiredReminderCount(int count)
{
    expiredReminders += count;

    QPushButton *remindersButton = MainWindow::window->ui->remindersButton;

    if (expiredReminders > 0)
    {
        remindersButton->setStyleSheet("QPushButton{background-color: #50772C2C} QPushButton:hover{background-color: #90732434}");
        remindersButton->setIcon(QIcon(":/Icons/Resources/Icons/Expired Reminder.svg"));
    }
    else
    {
        remindersButton->setIcon(QIcon(":/Icons/Resources/Icons/Reminder.svg"));
        remindersButton->setStyleSheet("");
    }
}

/**
 * @brief RemindersContainer::eventRenamed finds the reminder which was renamed and renames it
 */
void RemindersContainer::eventRenamed(QString oldPath, QString newPath)
{
    foreach (Reminder *reminder, reminders)
    {
        if (reminder->eventPath == oldPath)
        {
            reminder->setEventPath(newPath);
            return;
        }
    }
}

/**
 * @brief RemindersContainer::updateReminderTime finds the reminder which had its time changed and changes the time
 */
void RemindersContainer::updateReminderTime(QString eventPath, QDateTime reminderTime)
{
    foreach(Reminder *reminder, reminders)
    {
        if (reminder->eventPath == eventPath)
        {
            qDebug()<<"Here";
            reminder->setEventTime(reminderTime);
            qDebug()<<"Here";
            return;
        }
    }
}
