#include "eventdialog.h"
#include "ui_eventdialog.h"
#include "mainwindow.h"
#include "reminderscontainer.h"
#include "reminder.h"
#include "filemanager.h"
#include <QGraphicsDropShadowEffect>
#include <QDateTime>
#include <QTimer>
#include <QDir>

EventDialog::EventDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventDialog)
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

    this->ui->eventName->setFocus();
    this->ui->timeEdit->setEnabled(false);
    newEvent = false;
}

EventDialog::~EventDialog()
{
    delete ui;
}

void EventDialog::displayDialog(CalendarEvent *event, QPoint point)
{
    EventDialog::event = event;

    if (event->eventName != "")
    {
        this->ui->eventName->setText(event->eventName);
        if (event->reminderTime.isValid())
        {
            this->ui->checkBox->setChecked(true);
            this->ui->timeEdit->setTime(event->reminderTime);
        }
    }
    else
    {
        newEvent = true;
        if (event->eventDate < QDate::currentDate())    this->ui->checkBox->setEnabled(false);
        else if (event->eventDate == QDate::currentDate())
            this->ui->timeEdit->setMinimumTime(QTime(QTime::currentTime().hour(), QTime::currentTime().minute()));
    }

    this->move(calculatePosition(point));
    this->show();
}

/**
 * @brief EventDialog::calculatePosition calculates where to show the dialog based on click location
 * @param point
 * @return
 */
QPoint EventDialog::calculatePosition(QPoint point)
{
    QPoint topLeft, topRight, bottomRight;
    topLeft.setX(point.x() + 50);
    topLeft.setY(point.y() + 10);

    topRight.setX(topLeft.x() + width());
    topRight.setY(topLeft.y());
    if (!MainWindow::window->visibleRegion().contains(topRight))    // outside visible region
        topLeft.setX(point.x() - width());

    bottomRight.setX(topLeft.x() + width());
    bottomRight.setY(topLeft.y() + height());
    if (!MainWindow::window->visibleRegion().contains(bottomRight)) // outside visible region
        topLeft.setY(point.y() - height());
    return topLeft;
}

void EventDialog::closeEvent(QCloseEvent *closeEvent)
{
    if (newEvent)   createEvent();
    else            updateEvent();
    emit hidden();
    QWidget::closeEvent(closeEvent);
}

/**
 * @brief EventDialog::createEvent creates a new event with dialog information
 */
void EventDialog::createEvent()
{
    QString newText = this->ui->eventName->text();
    if (newText == "")  delete  event;
    else
    {
        event->setEventName(newText);
        if (this->ui->checkBox->isChecked())    setReminder();
        event->addToCalendar();
    }
}

/**
 * @brief EventDialog::updateEvent updates details of existing event with dialog information
 */
void EventDialog::updateEvent()
{
    QString newText = this->ui->eventName->text();
    if (newText != "")
    {
        // update calendar's tracker
        QString oldEventPath = event->parentPath + "/" + event->eventName + "/files.mar";
        QString newEventPath = event->parentPath + "/" + newText + "/files.mar";
        FileManager::updateFileTracker(event->parentPath + "/files.cal", "/" + event->eventName + "/files.mar", "/"+ newText + "/files.mar");
        // update directory name
        QDir dir(oldEventPath.section("/", 0, -2));
        dir.rename(oldEventPath.section("/", 0, -2), newEventPath.section("/", 0, -2));
        event->setEventName(newText);
        // update reminder if any
        if (event->reminderTime.isValid())
            RemindersContainer::eventRenamed(oldEventPath, newEventPath);
    }

    // reminder already exists
    if (event->reminderTime.isValid())
    {
        QString eventPath = event->parentPath.section("/", 0, -1) + "/" + event->eventName + "/files.mar";
        // update reminder time
        if (this->ui->checkBox->isChecked())
        {
            QDate date = event->eventDate;
            QTime time = this->ui->timeEdit->time();
            QDateTime reminderTime(date, time);
            RemindersContainer::updateReminderTime(eventPath, reminderTime);
        }
        else    // delete existing reminder
        {
            RemindersContainer::removeReminder(eventPath);
            event->reminderTime = QTime(-1, 0);
        }
    }
    else    // create new reminder
    {
        if (this->ui->checkBox->isChecked())    setReminder();
    }
    event->retrieveReminderTime();  // get the event to re-retrieve the new reminder time (if any)
}

/**
 * @brief EventDialog::setReminder creates a new reminder for the event
 */
void EventDialog::setReminder()
{
    QDate date = event->eventDate;
    QTime time = this->ui->timeEdit->time();
    QDateTime reminderTime(date, time);
    QString eventPath = event->parentPath + "/" + event->getEventName() + "/files.mar";
    RemindersContainer::createNewReminder(eventPath, reminderTime);
}

void EventDialog::on_checkBox_toggled(bool checked)
{
    if (checked)    this->ui->timeEdit->setEnabled(true);
    else            this->ui->timeEdit->setEnabled(false);
}

