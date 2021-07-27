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

    void retrieveEvents();

    QString selfPath;
    QList<QPair<QDate, QString>> events;

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

    QDate getDate(int row, int column);
public:
    Calendar(QString selfPath);

    void addEvents();
    void addToDate(QDate date, QLineEdit *textField);
    QLineEdit * createTextField(QString text);
public slots:
    void updateDates();
    void toPrevMonth();
    void toNextMonth();
    void addEvent(int row, int column);
};

#endif // CALENDAR_H
