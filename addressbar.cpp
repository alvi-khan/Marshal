#include "addressbar.h"
#include "ui_addressbar.h"

AddressBar::AddressBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AddressBar)
{
    ui->setupUi(this);
}

AddressBar::~AddressBar()
{
    delete ui;
}

void AddressBar::update(QString path)
{
    QLabel *label = new QLabel();
    label->setText(path);

    QLayoutItem *topItem = this->layout()->takeAt(0);
    while(topItem != NULL)
    {
        this->layout()->removeItem(topItem);
        topItem->widget()->deleteLater();
        topItem = this->layout()->takeAt(0);
    }

    this->layout()->addWidget(label);
}
