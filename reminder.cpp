#include "reminder.h"
#include "ui_reminder.h"
#include "filemanager.h"

#include <QTimer>

Reminder::Reminder(QWidget *parent, QString eventPath, QDateTime reminderTime) :
    QWidget(parent),
    ui(new Ui::Reminder)
{
    ui->setupUi(this);
    this->ui->eventName->setText(eventPath.section("/", -2, -2));
    this->ui->date->setText(reminderTime.date().toString("dd/MM/yyyy"));
    this->ui->time->setText(reminderTime.time().toString("hh:mm"));
    Reminder::eventPath = eventPath;
    Reminder::reminderTime = reminderTime;
    setTimer();
}

Reminder::~Reminder()
{
    delete ui;
}

void Reminder::setTimer()
{
    QTimer *timer = new QTimer();
    int millisecondsDiff = QDateTime::currentDateTime().msecsTo(reminderTime);
    timer->singleShot(millisecondsDiff, [=]{
        this->setStyleSheet("QFrame#container{border: 1.5px solid #772C2C;background-color: rgb(35, 35, 35);}");
    });
}
