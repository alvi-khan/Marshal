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
    static void removeReminder(Reminder *reminder);
    static void deleteReminder(QString eventPath, QDateTime dateTime);
    static void createNewReminder(QString eventPath, QDateTime dateTime);
    static void retrieveReminders();
    static void hideContainer();
    static void updateExpiredReminderCount(int count);
    ~RemindersContainer();

private:
    Ui::RemindersContainer *ui;
    static QList<Reminder *> reminders;
    static RemindersContainer *activeContainer;
    static int expiredReminders;
    void init();
};

#endif // REMINDERSCONTAINER_H
