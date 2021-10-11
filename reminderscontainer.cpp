#include "reminderscontainer.h"
#include "ui_reminderscontainer.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reminder.h"

#include <QGraphicsDropShadowEffect>

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
    Reminder *reminder1 = new Reminder();
    Reminder *reminder2 = new Reminder();
    Reminder *reminder3 = new Reminder();
    Reminder *reminder4 = new Reminder();
    Reminder *reminder5 = new Reminder();
    Reminder *reminder6 = new Reminder();

    QLayout *layout = this->ui->mainArea->layout();
    layout->addWidget(reminder1);
    layout->addWidget(reminder2);
    layout->addWidget(reminder3);
    layout->addWidget(reminder4);
    layout->addWidget(reminder5);
    layout->addWidget(reminder6);
    //QPushButton *remindersButton = MainWindow::window->ui->remindersButton;
    QPoint point = MainWindow::window->ui->remindersButton->mapToGlobal(MainWindow::window->ui->remindersButton->rect().topRight());

    this->move(point.x() + 20, point.y());
    this->show();
    this->adjustSize();
}
