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

class Calendar : public QTableWidget
{
    Q_OBJECT
    void createTable(int rows, int columns);
    void hideDefaultHeaders();
    void customizeCalendar();
    void createButtons();
    void createMonthPicker();
    void insertDayNames();
    void populateDates();

    void clearCells();
    QWidget * createDateText(int date);
    QComboBox * newCustomComboBox();
    void heightReset();
    int weekInMonth(QDate date);

    QStringList weekdays = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    QStringList months = {"January", "February", "March", "April", "May", "June",
                              "July", "August", "September", "October", "November", "December"};
    QComboBox *monthList;
    QComboBox *yearList;

    int yearRangeStart;
    int yearRangeEnd;

    QDate today;
    int selectedMonth;
    int selectedYear;

public:
    Calendar();

public slots:
    void updateDates();
    void toPrevMonth();
    void toNextMonth();
};

#endif // CALENDAR_H
