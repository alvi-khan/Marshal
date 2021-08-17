#include "eventdialog.h"
#include "ui_eventdialog.h"

EventDialog::EventDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

EventDialog::~EventDialog()
{
    delete ui;
}

void EventDialog::displayDialog(CalendarEvent *event, QPoint point)
{
    EventDialog::event = event;
    QPoint position = event->mapToGlobal(point);
    this->move(position.x() + 50, position.y() + 10);
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
    }
}

