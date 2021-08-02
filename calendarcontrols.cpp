#include "calendarcontrols.h"

CalendarControls::CalendarControls(Calendar *calendar)
{
    this->calendar = calendar;
    yearRangeStart = 2000;
    yearRangeEnd = 2100;

    createButtons();
    createMonthPicker();
}

void CalendarControls::createButtons()
{
    QPushButton *prevButton = new QPushButton(QIcon(":/Icons/Resources/Icons/Left Arrow.svg"), "");
    calendar->setCellWidget(0, 0, prevButton);
    prevButton->setStyleSheet("border: none;");
    connect(prevButton, &QPushButton::clicked, this, &CalendarControls::toPrevMonth);

    QPushButton *nextButton = new QPushButton(QIcon(":/Icons/Resources/Icons/Right Arrow.svg"), "");
    calendar->setCellWidget(0, 6, nextButton);
    nextButton->setStyleSheet("border: none;");
    connect(nextButton, &QPushButton::clicked, this, &CalendarControls::toNextMonth);
}

void CalendarControls::createMonthPicker()
{
    QList<QString> years;
    for (int i=yearRangeStart; i<=yearRangeEnd; i++)
        years.append(QString::number(i));
    yearList = newCustomComboBox(years, calendar->selectedYear - yearRangeStart);

    monthList = newCustomComboBox(calendar->months, calendar->selectedMonth - 1);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addStretch();
    layout->addWidget(monthList);
    layout->addWidget(yearList);
    layout->addStretch();
    QWidget *comboBoxes = new QWidget();
    comboBoxes->setLayout(layout);

    //QWidget *comboBoxes = createWidgetWithItems(new QHBoxLayout(), {new QSpacerItem(), monthList, yearList, })

    calendar->setCellWidget(0, 1, comboBoxes);
    calendar->setSpan(0, 1, 1, 5);
}

QComboBox * CalendarControls::newCustomComboBox(QList<QString> itemList, int currentItem)
{
    QComboBox *comboBox = new QComboBox();
    comboBox->addItems(itemList);
    comboBox->setCurrentIndex(currentItem);
    connect(comboBox, &QComboBox::currentIndexChanged, this, &CalendarControls::updateComboBoxSelections);

    comboBox->setFrame(QFrame::NoFrame);
    comboBox->setStyleSheet("QComboBox::drop-down {border-width: 0px;} QComboBox::down-arrow {image: url(noimg); border-width: 0px;}");
    comboBox->setFocusPolicy(Qt::StrongFocus);  // ignored mouse scroll when scrolling page (but not on hover)
    return comboBox;
}

void CalendarControls::toPrevMonth()
{
    if (calendar->selectedMonth == 1 && calendar->selectedYear == yearRangeStart)
        return; // outside range
    else if (calendar->selectedMonth == 1)    // decrementing January
    {
        monthList->setCurrentIndex(11);
        yearList->setCurrentIndex(yearList->currentIndex() - 1);
    }
    else    monthList->setCurrentIndex(monthList->currentIndex() - 1);
    calendar->heightReset();
}

void CalendarControls::toNextMonth()
{
    if (calendar->selectedMonth == 12 && calendar->selectedYear == yearRangeEnd)    return; // outside range
    else if (calendar->selectedMonth == 12) // incrementing December
    {
        monthList->setCurrentIndex(0);
        yearList->setCurrentIndex(yearList->currentIndex() + 1);
    }
    else    monthList->setCurrentIndex(monthList->currentIndex() + 1);
    calendar->heightReset();
}

void CalendarControls::updateComboBoxSelections()
{
    calendar->selectedMonth = monthList->currentIndex() + 1;
    calendar->selectedYear = yearList->currentIndex() + yearRangeStart;
    calendar->populateDates();
    calendar->heightReset();
}
