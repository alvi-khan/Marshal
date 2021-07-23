#include "error.h"
#include "ui_error.h"

Error::Error(QWidget *parent, QString errorMessage) :
    QDialog(parent),
    ui(new Ui::Error)
{
    ui->setupUi(this);
    this->ui->errorMessage->setPlainText(errorMessage);
}

Error::~Error()
{
    delete ui;
}

void Error::showError()
{

}

bool Error::errorAccepted() const
{
    return errorAcceptanceStatus;
}

void Error::on_buttonBox_accepted()
{
    errorAcceptanceStatus = true;
}

void Error::on_buttonBox_rejected()
{
    errorAcceptanceStatus = false;
}

