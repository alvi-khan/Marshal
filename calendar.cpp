#include "calendar.h"
#include "newevent.h"
#include "filemanager.h"
#include "calendarevent.h"
#include "calendarcontrols.h"

#include <QDir>
#include <QFile>

Calendar::Calendar(QString selfPath)
{
    this->selfPath = selfPath;
    retrieveEvents();

    today = QDate::currentDate();
    selectedMonth = today.month();
    selectedYear = today.year();

    createCalendarTable(7, 7);  // 5 weeks + 2 hears and 7 days
    CalendarControls *controls = new CalendarControls(this);
    //delete controls;
    insertDayNames();
    populateDates();
    heightReset();
    connect(this, &QTableWidget::cellClicked, this, &Calendar::addNewEvent);
}

void Calendar::createCalendarTable(int rows, int columns)
{
    this->setRowCount(rows);
    this->setColumnCount(columns);
    customizeCalendar();
}

void Calendar::hideDefaultHeaders()
{
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setVisible(false);
}

void Calendar::customizeCalendar()
{
    hideDefaultHeaders();

    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setFrameStyle(QFrame::NoFrame);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setMaximumWidth(this->width());
    this->setShowGrid(false);

    this->setStyleSheet("color: #FFFFFF; "
                        "background-color: rgb(47, 52, 55);");
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
    populateMonthEvents();
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

void Calendar::setCurrentDateFormat(QLineEdit *dateText)
{
    dateText->setStyleSheet("background-color: rgb(211, 82, 93); "  // red
                            "border: 1px solid rgb(47, 52, 55); "   // same as page (hidden border)
                            "border-radius: 10px; "
                            "max-width: 18px;");
}

QWidget* Calendar::createWidgetWithItems(QBoxLayout *layout, QList<QWidget *> widgets)
{
    foreach (QWidget *widget, widgets)
        layout->addWidget(widget);
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    return widget;
}

QWidget* Calendar::createDateText(int date)
{
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setText(QString::number(date));
    lineEdit->setFrame(QFrame::NoFrame);
    lineEdit->setReadOnly(true);

    if (today.year() == selectedYear && today.month() == selectedMonth && date == today.day())
        setCurrentDateFormat(lineEdit);


    QWidget *widget = createWidgetWithItems(new QVBoxLayout(), {lineEdit});
    widget->layout()->setAlignment(Qt::AlignTop);
    return widget;
}

void Calendar::addToEventList(QDate date, QString eventName)
{
    QPair<QDate, QString> newEvent;
    newEvent.first = date;
    newEvent.second = eventName;
    events.append(newEvent);
}

void Calendar::retrieveEvents()
{
    QFile file(selfPath);
    file.open(QIODevice::ReadOnly);
    QTextStream data(&file);
    while (!data.atEnd())
    {
        QDate date = QDate::fromString(data.readLine(), "dd/MM/yyyy");
        QString event = data.readLine();
        addToEventList(date, event);
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

void Calendar::addToDateCell(QDate date, QLineEdit *textField)
{
    int row = weekInMonth(date) + 2;
    int column = date.dayOfWeek() - 1;

    QVBoxLayout *layout = (QVBoxLayout *) this->cellWidget(row, column)->layout();
    QWidget *widget = createWidgetWithItems(layout, {textField});

    this->removeCellWidget(row, column);
    this->setCellWidget(row, column, widget);
}

void Calendar::populateMonthEvents()
{
    foreach (auto event, events)
    {
        QDate date = event.first;
        QString path = event.second;
        if (date.month() == selectedMonth && date.year() == selectedYear)
        {
            QString eventName = path.section("/", -2, -2);
            CalendarEvent *eventWidget = new CalendarEvent(selfPath, date, eventName);
            addToDateCell(date, eventWidget);
        }
    }
}

QDate Calendar::getDateFromCell(int row, int column)
{
    QDate date;
    if (row < 2)    return date;
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

void Calendar::addNewEvent(int row, int column)
{

    QDate date = getDateFromCell(row, column);
    if (!date.isValid())    return;

    CalendarEvent *newEvent = new CalendarEvent(selfPath, date);

    if (newEvent->text() != "")
    {
        addToDateCell(date, newEvent);
        heightReset();
        newEvent->saveToDisk();
        qDebug()<<newEvent->getEventFilePath();
        addToEventList(date, newEvent->getEventFilePath());
    }
    else    delete newEvent;
}
