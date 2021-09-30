#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class AddressBar;
}

class AddressBar : public QFrame
{
    Q_OBJECT

public:
    explicit AddressBar(QWidget *parent = nullptr);
    ~AddressBar();
    void update(QString path);

private:
    Ui::AddressBar *ui;
};

#endif // ADDRESSBAR_H
