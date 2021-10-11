#ifndef REMINDERSCONTAINER_H
#define REMINDERSCONTAINER_H

#include <QDialog>

namespace Ui {
class RemindersContainer;
}

class RemindersContainer : public QDialog
{
    Q_OBJECT

public:
    explicit RemindersContainer(QWidget *parent = nullptr);
    ~RemindersContainer();

private:
    Ui::RemindersContainer *ui;
    void init();
};

#endif // REMINDERSCONTAINER_H
