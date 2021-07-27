#include "calendar.h"
#include "newevent.h"
#include "filemanager.h"
#include "calendarevent.h"

#include <QDir>
#include <QFile>

Calendar::Calendar(QString selfPath)
{
    this->selfPath = selfPath;
    retrieveEvents();

    yearRangeStart = 2000;
    yearRangeEnd = 2100;
    today = QDate::currentDate();
    selectedMonth = today.month();
    selectedYear = today.year();

    createTable(7, 7);  // 5 weeks + 2 hears and 7 days
    hideDefaultHeaders();
    customizeCalendar();
    createButtons();
    createMonthPicker();
    insertDayNames();
    populateDates();
    heightReset();
    connect(this, &QTableWidget::cellClicked, this, &Calendar::addEvent);
}

void Calendar::createTable(int rows, int columns)
{
    this->setRowCount(rows);
    this->setColumnCount(columns);
}

void Calendar::hideDefaultHeaders()
{
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setVisible(false);
}

void Calendar::customizeCalendar()
{
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setFrameStyle(QFrame::NoFrame);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setMaximumWidth(this->width());
    this->setShowGrid(false);

    this->setStyleSheet("color: #FFFFFF; "
                        "background-color: rgb(47, 52, 55);");
}

void Calendar::createButtons()
{
    QPushButton *prevButton = new QPushButton(QIcon(":/Icons/Resources/Icons/Left Arrow.svg"), "");
    this->setCellWidget(0, 0, prevButton);
    prevButton->setStyleSheet("border: none;");
    connect(prevButton, &QPushButton::clicked, this, &Calendar::toPrevMonth);

    QPushButton *nextButton = new QPushButton(QIcon(":/Icons/Resources/Icons/Right Arrow.svg"), "");
    this->setCellWidget(0, 6, nextButton);
    nextButton->setStyleSheet("border: none;");
    connect(nextButton, &QPushButton::clicked, this, &Calendar::toNextMonth);
}

void Calendar::createMonthPicker()
{
    monthList = newCustomComboBox();
    monthList->addItems(months);
    monthList->setCurrentIndex(selectedMonth - 1);
    connect(monthList, &QComboBox::currentIndexChanged, this, &Calendar::updateDates);

    yearList = newCustomComboBox();
    for (int i=yearRangeStart; i<=yearRangeEnd; i++)   yearList->addItem(QString::number(i));
    yearList->setCurrentIndex(selectedYear - yearRangeStart);
    connect(yearList, &QComboBox::currentIndexChanged, this, &Calendar::updateDates);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addStretch();
    layout->addWidget(monthList);
    layout->addWidget(yearList);
    layout->addStretch();
    QWidget *comboBoxes = new QWidget();
    comboBoxes->setLayout(layout);

    this->setCellWidget(0, 1, comboBoxes);
    this->setSpan(0, 1, 1, 5);
}

void Calendar::insertDayNames()
{
    for (int i = 0; i<7; i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(weekdays.at(i));
        item->setTextAlignment(Qt::AlignHCenter);
        this->setItem(1, i, item);
    }
    this->verticalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    this->verticalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

void Calendar::populateDates()
{
    clearCells();

    QDate date(selectedYear, selectedMonth, 1);
    QDate end(selectedYear, selectedMonth, date.daysInMonth());

    while (date <= end)
    {
        int row = weekInMonth(date) + 2;
        int column = date.dayOfWeek() - 1;

        QWidget *widget = createDateText(date.day());
        this->setCellWidget(row, column, widget);

        date = date.addDays(1);
    }
    addEvents();
}

QComboBox * Calendar::newCustomComboBox()
{
    QComboBox *comboBox = new QComboBox();
    comboBox->setFrame(QFrame::NoFrame);
    comboBox->setStyleSheet("QComboBox::drop-down {border-width: 0px;} QComboBox::down-arrow {image: url(noimg); border-width: 0px;}");
    comboBox->setFocusPolicy(Qt::StrongFocus);  // ignored mouse scroll when scrolling page (but not on hover)
    return comboBox;
}

void Calendar::heightReset()
{
    this->resizeRowsToContents();
    int total = 0;
    for (int i=0; i<7; i++) total += this->rowHeight(i);
    this->setMinimumHeight(total + 10);
    this->setMaximumHeight(total + 10);
}

void Calendar::clearCells()
{
    for (int i=2; i<7; i++)
        for (int j=0; j<7; j++)
            while (cellWidget(i, j))
                removeCellWidget(i, j);
}

int Calendar::weekInMonth(QDate date)
{
    QDate monthStart(date.year(), date.month(), 1);
    // condition required due to weird bug in weekNumber method
    if (date.weekNumber() >= monthStart.weekNumber())
        return (date.weekNumber() - monthStart.weekNumber());
    else    return date.weekNumber();
}

QWidget* Calendar::createDateText(int date)
{
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setText(QString::number(date));
    lineEdit->setFrame(QFrame::NoFrame);
    lineEdit->setReadOnly(true);

    if (today.year() == selectedYear && today.month() == selectedMonth && date == today.day())
        lineEdit->setStyleSheet("background-color: rgb(211, 82, 93); "  // red
                                "border: 1px solid rgb(47, 52, 55); "   // same as page (hidden border)
                                "border-radius: 10px; "
                                "max-width: 18px;");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(lineEdit);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    return widget;
}

void Calendar::retrieveEvents()
{
    QFile file(selfPath);
    file.open(QIODevice::ReadOnly);
    QTextStream data(&file);
    while (!data.atEnd())
    {
        QPair<QDate, QString> newEvent;
        newEvent.first = QDate::fromString(data.readLine(), "dd/MM/yyyy");
        newEvent.second = data.readLine();
        events.append(newEvent);
    }
    file.close();
}

QLineEdit * Calendar::createTextField(QString text)
{
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setText(text);
    lineEdit->setFrame(QFrame::NoFrame);
    lineEdit->setReadOnly(true);

    lineEdit->setStyleSheet("background-color: rgb(42, 202, 124); "  // green
                            "border: 1px solid rgb(47, 52, 55); "   // same as page (hidden border)
                            "border-radius: 5px;");

    return lineEdit;
}

void Calendar::addToDate(QDate date, QLineEdit *textField)
{
    int row = weekInMonth(date) + 2;
    int column = date.dayOfWeek() - 1;
    QVBoxLayout *layout = (QVBoxLayout *) this->cellWidget(row, column)->layout();
    layout->addWidget(textField);
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    this->removeCellWidget(row, column);
    this->setCellWidget(row, column, widget);
}

void Calendar::addEvents()
{
    foreach (auto event, events)
    {
        QDate date = event.first;
        QString path = event.second;
        if (date.month() == selectedMonth && date.year() == selectedYear)
        {
            CalendarEvent *textField = new CalendarEvent(path.section("/", -2, -2), selfPath);
            addToDate(date, textField);
        }
    }
}

void Calendar::updateDates()
{
    selectedMonth = monthList->currentIndex() + 1;
    selectedYear = yearList->currentIndex() + yearRangeStart;
    populateDates();
    heightReset();
}

void Calendar::toPrevMonth()
{
    if (selectedMonth == 1 && selectedYear == yearRangeStart)    return; // outside range
    else if (selectedMonth == 1)    // decrementing January
    {
        monthList->setCurrentIndex(11);
        yearList->setCurrentIndex(yearList->currentIndex() - 1);
    }
    else    monthList->setCurrentIndex(monthList->currentIndex() - 1);
    heightReset();
}

void Calendar::toNextMonth()
{
    if (selectedMonth == 12 && selectedYear == yearRangeEnd)    return; // outside range
    else if (selectedMonth == 12) // incrementing December
    {
        monthList->setCurrentIndex(0);
        yearList->setCurrentIndex(yearList->currentIndex() + 1);
    }
    else    monthList->setCurrentIndex(monthList->currentIndex() + 1);
    heightReset();
}

QDate Calendar::getDate(int row, int column)
{
    QDate date;
    QWidget *widget = this->cellWidget(row, column);
    if (widget != nullptr)
    {
        QVBoxLayout *layout = (QVBoxLayout *) widget->layout();
        QLineEdit *dateText = (QLineEdit *) layout->itemAt(0)->widget();
        int dateNum = dateText->text().toInt();
        date.setDate(selectedYear, selectedMonth, dateNum);
    }
    return date;
}

void Calendar::addEvent(int row, int column)
{
    QDate date = getDate(row, column);
    if (!date.isValid())    return;

    NewEvent *newEvent = new NewEvent();
    newEvent->exec();

    if (!newEvent->accepted || newEvent->fileName == "")    return;

    CalendarEvent *eventText = new CalendarEvent(newEvent->fileName, selfPath);
    addToDate(date, eventText);
    heightReset();
    delete newEvent;

    QString eventPath = "/" + eventText->text();
    QDir dir(selfPath.section("/", 0, -2) + eventPath);
    dir.mkpath(dir.path());
    FileManager::readFromFile(dir.path() + "/files.mar");
    FileManager::appendToFile(selfPath, date.toString("dd/MM/yyyy\n"));
    FileManager::appendToFile(selfPath, eventPath + "/files.mar\n");

    QPair<QDate, QString> listEvent;
    listEvent.first = date;
    listEvent.second = eventPath + "/files.mar";
    events.append(listEvent);
}
