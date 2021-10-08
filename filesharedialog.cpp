#include "filesharedialog.h"
#include "ui_filesharedialog.h"
#include "databasemanager.h"
#include <QCompleter>

FileShareDialog::FileShareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileShareDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowIcon(QIcon(":/Toolbar Icons/Resources/Toolbar Icons/Share.svg"));
    this->setWindowTitle("Share");
    init();
}

FileShareDialog::~FileShareDialog()
{
    delete ui;
}

void FileShareDialog::on_buttonBox_accepted()
{
    username = this->ui->username->text();
    accept();
}


void FileShareDialog::on_buttonBox_rejected()
{
    reject();
}

void FileShareDialog::init()
{
    username = "";
}


void FileShareDialog::on_username_textChanged(const QString &arg1)
{
    QList<QString> users = DatabaseManager::getUserList();


    QCompleter *userSuggestions = new QCompleter(users, this);
    userSuggestions->setCaseSensitivity(Qt::CaseInsensitive);
    QAbstractItemView *list = userSuggestions->popup();
    list->setStyleSheet("color: #FFFFFF; font-size: 12px; background-color: rgb(35, 35, 35);");
    userSuggestions->setPopup(list);
    this->ui->username->setCompleter(userSuggestions);
}

