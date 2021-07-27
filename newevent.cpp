#include "newevent.h"
#include "ui_newevent.h"

NewEvent::NewEvent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewEvent)
{
    ui->setupUi(this);
    init();
}

void NewEvent::init()
{
    this->setWindowIcon(QIcon(":/Icons/Resources/Icons/Marshal Logo.svg"));
    this->setWindowTitle("New Event");

    this->ui->fileName->setFocus();
    fileName = "";
    accepted = false;
}

NewEvent::~NewEvent()
{
    delete ui;
}

void NewEvent::on_buttonBox_accepted()
{
    accepted = true;
    fileName = this->ui->fileName->text();
}

