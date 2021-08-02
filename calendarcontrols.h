#ifndef CALENDARCONTROLS_H
#define CALENDARCONTROLS_H

#include "calendar.h"


class CalendarControls : public QObject
{
    Q_OBJECT
public:
    CalendarControls(Calendar *calendar);
    Calendar *calendar;
    QComboBox *monthList;
    QComboBox *yearList;
    int yearRangeStart;
    int yearRangeEnd;
    void createButtons();
    void createMonthPicker();
    QComboBox *newCustomComboBox(QList<QString> itemList, int currentItem);
    void toPrevMonth();
    void toNextMonth();
public slots:
    void updateComboBoxSelections();
};

#endif // CALENDARCONTROLS_H
