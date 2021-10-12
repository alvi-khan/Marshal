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
    QString eventPath;
    QDateTime reminderTime;
    void setEventPath(QString eventPath);

private:
    Ui::Reminder *ui;
    void setTimer();
    void expire();
    bool expired;
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // REMINDER_H
