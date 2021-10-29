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

/**
 * @brief AddressBar::update updates the content of the address bar
 * @param path relative path from home directory
 */
void AddressBar::update(QString path)
{
    /* Note that this function is partially setup to accomodate
       a multi-part, clickable addrress in the future.
    */

    QLabel *label = new QLabel();
    label->setText(path);

    // Removing all parts of the address bar.
    QLayoutItem *topItem = this->layout()->takeAt(0);
    while(topItem != NULL)
    {
        this->layout()->removeItem(topItem);
        topItem->widget()->deleteLater();
        topItem = this->layout()->takeAt(0);
    }

    this->layout()->addWidget(label);
}
