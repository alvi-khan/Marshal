#include "logindialog.h"
#include "ui_logindialog.h"
#include "databasemanager.h"

#include <QPushButton>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    init();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtn_clicked()
{
    QString username = this->ui->username->text();
    QString password = this->ui->password->text();
    if (username == "" || password == "")   return;
    if (!DatabaseManager::authenticateUser(username, password))
    {
        QMessageBox::warning(this, "Login Failed", "Incorrect username or password!");
    }
    else
        accept();
}


void LoginDialog::on_regBtn_clicked()
{
    QString username = this->ui->username->text();
    QString password = this->ui->password->text();
    if (username == "" || password == "")   return;
    if (!DatabaseManager::createNewUser(username, password))
    {
        QMessageBox::warning(this, "Registration Failed", "Username unavailable!");
    }
    else
        accept();
}


void LoginDialog::on_cancelBtn_clicked()
{
    reject();
}


void LoginDialog::on_logoutBtn_clicked()
{
    DatabaseManager::logoutUser();

    this->ui->logoutBtn->hide();
    this->ui->welcomeLabel->hide();

    this->ui->usernameLabel->show();
    this->ui->username->show();
    this->ui->passwordLabel->show();
    this->ui->password->show();
    this->ui->loginBtn->show();
    this->ui->regBtn->show();
}

void LoginDialog::init()
{
    if (DatabaseManager::username == "")
    {
        this->ui->logoutBtn->hide();
        this->ui->welcomeLabel->hide();
    }
    else
    {
        this->ui->usernameLabel->hide();
        this->ui->username->hide();
        this->ui->passwordLabel->hide();
        this->ui->password->hide();
        this->ui->loginBtn->hide();
        this->ui->regBtn->hide();

        this->ui->welcomeLabel->setText("Welcome " + DatabaseManager::username + "!");
    }
}

