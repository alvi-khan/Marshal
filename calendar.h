#ifndef CALENDAR_H
#define CALENDAR_H

#include <QDate>
#include <QObject>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QPair>

class Calendar : public QTableWidget
{
    Q_OBJECT
public:
    void createCalendarTable(int rows, int columns);
    void hideDefaultHeaders();
    void customizeCalendar();
    void insertDayNames();
    void populateDates();

    void clearCells();
    QWidget * createDateText(int date);
    void heightReset();
    int weekInMonth(QDate date);

    void retrieveEvents();

    QString selfPath;
    QList<QPair<QDate, QString>> events;

    QStringList weekdays = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    QStringList months = {"January", "February", "March", "April", "May", "June",
                              "July", "August", "September", "October", "November", "December"};

    QDate today;
    int selectedMonth;
    int selectedYear;

    QDate getDateFromCell(int row, int column);
public:
    Calendar(QString selfPath);

    void populateMonthEvents();
    void addToDateCell(QDate date, QWidget *textField);
    void addToEventList(QDate date, QString eventName);
    void setCurrentDateFormat(QLineEdit *dateText);
    QWidget *createWidgetWithItems(QBoxLayout *layout, QList<QWidget*> widgets);
public slots:
    void newEventPrompt(int row, int column);
    void clearSelectedCell(QWidget *cellWidget);
};

#endif // CALENDAR_H
