#ifndef REMINDERSCONTAINER_H
#define REMINDERSCONTAINER_H

#include <QDialog>
#include "reminder.h"

namespace Ui {
class RemindersContainer;
}

class RemindersContainer : public QDialog
{
    Q_OBJECT

public:
    explicit RemindersContainer(QWidget *parent = nullptr);
    static void addReminder(QString eventPath, QDateTime dateTime);
    static void createNewReminder(QString eventPath, QDateTime dateTime);
    static void retrieveReminders();
    ~RemindersContainer();

private:
    Ui::RemindersContainer *ui;
    static QList<Reminder *> reminders;
    void init();
};

#endif // REMINDERSCONTAINER_H
