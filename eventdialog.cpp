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
}

EventDialog::~EventDialog()
{
    delete ui;
}

void EventDialog::displayDialog(CalendarEvent *event, QPoint point)
{
    EventDialog::event = event;

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

void EventDialog::on_eventName_editingFinished()
{
    QString newText = this->ui->eventName->text();
    if (newText == "")  delete  event;
    else
    {
        event->setText(newText);
        event->addToCalendar();
        setReminder();
    }
    this->hide();
    emit hidden();
}

void EventDialog::setReminder()
{
    QDateTime reminderTime(QDate(2021, 10, 12), QTime(00, 21));
    QString eventPath = event->parentPath + "/" + event->text() + "/files.mar";
    RemindersContainer::createNewReminder(eventPath, reminderTime);
}
