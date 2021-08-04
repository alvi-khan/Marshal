#include "filesharedialog.h"
#include "ui_filesharedialog.h"

FileShareDialog::FileShareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileShareDialog)
{
    ui->setupUi(this);
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

