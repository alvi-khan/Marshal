#ifndef REMINDER_H
#define REMINDER_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class Reminder;
}

class Reminder : public QWidget
{
    Q_OBJECT

public:
    explicit Reminder(QWidget *parent = nullptr, QString eventPath = "", QDateTime reminderTime = QDateTime::currentDateTime());
    ~Reminder();

private:
    Ui::Reminder *ui;
    QString eventPath;
    QDateTime reminderTime;
    void setTimer();
};

#endif // REMINDER_H
