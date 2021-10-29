#ifndef REMINDER_H
#define REMINDER_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class Reminder;
}

/**
 * @brief The Reminder class handles individual reminders stored in the reminder container
 */
class Reminder : public QWidget
{
    Q_OBJECT

public:
    explicit Reminder(QWidget *parent = nullptr, QString eventPath = "", QDateTime reminderTime = QDateTime::currentDateTime());
    ~Reminder();
    QString eventPath;
    QDateTime reminderTime;
    void setEventPath(QString eventPath);
    void setEventTime(QDateTime dateTime);

private:
    Ui::Reminder *ui;
    void setTimer();
    void expire();
    void unexpire();
    bool expired;
    QTimer *timer;
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // REMINDER_H
