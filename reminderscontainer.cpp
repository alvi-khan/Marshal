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
    foreach(Reminder *reminder, reminders)
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

void RemindersContainer::addReminder(QString eventPath, QDateTime dateTime)
{
    Reminder *reminder = new Reminder(MainWindow::window, eventPath, dateTime);
    reminders.append(reminder);
    reminder->hide();
}

void RemindersContainer::removeReminder(Reminder *reminder)
{
    reminders.removeOne(reminder);
    deleteReminder(reminder->eventPath, reminder->reminderTime);
    updateExpiredReminderCount(-1);
}

void RemindersContainer::deleteReminder(QString eventPath, QDateTime dateTime)
{
    QString remindersStorage = QCoreApplication::applicationDirPath() + "/reminders.dat";
    QString data = FileManager::readFromFile(remindersStorage);
    data.remove(dateTime.toString() + "\n" + eventPath + "\n");
    FileManager::writeToFile(remindersStorage, data);
}

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
