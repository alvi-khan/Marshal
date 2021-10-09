#include "error.h"
#include "ui_error.h"
#include "mainwindow.h"
#include <QGraphicsBlurEffect>

Error::Error(QWidget *parent, QString errorMessage) :
    QDialog(parent),
    ui(new Ui::Error)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowIcon(QIcon(":/Icons/Resources/Icons/Error.svg"));
    this->setWindowTitle("Error");
    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect();
    blurEffect->setBlurRadius(10);
    MainWindow::window->setGraphicsEffect(blurEffect);
    this->ui->errorMessage->setText(errorMessage);
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


void Error::on_okButton_clicked()
{
     errorAcceptanceStatus = true;
     this->close();
}


void Error::on_cancelButton_clicked()
{
    errorAcceptanceStatus = false;
    this->close();
}

void Error::closeEvent(QCloseEvent *event)
{
    MainWindow::window->setGraphicsEffect(nullptr);
    this->accept();
}

