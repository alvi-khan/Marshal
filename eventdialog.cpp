#include "eventdialog.h"
#include "ui_eventdialog.h"
#include "mainwindow.h"
#include "reminderscontainer.h"
#include "reminder.h"
#include <QGraphicsDropShadowEffect>
#include <QDateTime>
#include <QTimer>

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
}

EventDialog::~EventDialog()
{
    delete ui;
}

void EventDialog::displayDialog(CalendarEvent *event, QPoint point)
{
    EventDialog::event = event;

    if (event->eventDate < QDate::currentDate())
    {
        this->ui->checkBox->setEnabled(false);
        this->ui->checkBox->setStyleSheet("QCheckBox:disabled{color: rgb(70, 70, 70);}\
                                           QCheckBox::indicator {height: 16px; width: 16px;\
                                                border-image : url(:/Icons/Resources/Icons/CheckBox Disabled.svg);}");
    }
    else if (event->eventDate == QDate::currentDate()) (this->ui->timeEdit->setMinimumTime(QTime::currentTime()));

    QPoint topLeft, topRight, bottomRight;
    topLeft.setX(point.x() + 50);
    topLeft.setY(point.y() + 10);

    topRight.setX(topLeft.x() + width());
    topRight.setY(topLeft.y());
    if (!MainWindow::window->visibleRegion().contains(topRight))
        topLeft.setX(point.x() - width());

    bottomRight.setX(topLeft.x() + width());
    bottomRight.setY(topLeft.y() + height());
    if (!MainWindow::window->visibleRegion().contains(bottomRight))
        topLeft.setY(point.y() - height());

    this->move(topLeft);
    this->show();
}

void EventDialog::closeEvent(QCloseEvent *closeEvent)
{
    QString newText = this->ui->eventName->text();
    if (newText == "")  delete  event;
    else
    {
        event->setEventName(newText);
        event->addToCalendar();
        if (this->ui->checkBox->isChecked())    setReminder();
    }
    emit hidden();
    QWidget::closeEvent(closeEvent);
}

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

