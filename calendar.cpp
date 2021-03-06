#include "calendar.h"
#include "eventdialog.h"
#include "filemanager.h"
#include "calendarcontrols.h"
#include "reminderscontainer.h"

#include <QCoreApplication>
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
    insertDayNames();
    populateDates();
    heightReset();
    connect(this, &QTableWidget::cellClicked, this, &Calendar::newEventPrompt);
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

    this->setStyleSheet("QTableWidget::item{selection-background-color: rgb(35, 35, 35);}");
    this->setFocusPolicy(Qt::NoFocus);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

/**
 * @brief Calendar::populateDates retrieves all data for the current month's dates
 */
void Calendar::populateDates()
{
    clearCells();

    QDate date(selectedYear, selectedMonth, 1);
    QDate end(selectedYear, selectedMonth, date.daysInMonth());
    // adding dates
    while (date <= end)
    {
        int row = weekInMonth(date) + 2;
        int column = date.dayOfWeek() - 1;

        QWidget *widget = createDateText(date.day());
        widget->setStyleSheet("*:hover {background: #3E3E3E; border-radius: 10px}");
        this->setCellWidget(row, column, widget);
        date = date.addDays(1);
    }
    populateMonthEvents();
}

/**
 * @brief Calendar::heightReset re-adjusts heights of calendar rows
 */
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

/**
 * @brief Calendar::weekInMonth retrieves week number for a particular date in the current month
 */
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
                            "border: 1px solid rgb(211, 82, 93); "   // same as circle (hidden border)
                            "border-radius: 10px; "
                            "max-width: 18px;");
}

/**
 * @brief Calendar::createWidgetWithItems creates a widget with a layout containing a list of widgets
 */
QWidget* Calendar::createWidgetWithItems(QBoxLayout *layout, QList<QWidget *> widgets)
{
    foreach (QWidget *widget, widgets)
        layout->addWidget(widget);
    QWidget *widget = new QWidget();
    widget->setStyleSheet("*:hover {background: #3E3E3E; border-radius: 10px;}");
    widget->setLayout(layout);
    return widget;
}

QWidget* Calendar::createDateText(int date)
{
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setText(QString::number(date));
    lineEdit->setFrame(QFrame::NoFrame);
    lineEdit->setReadOnly(true);
    lineEdit->setStyleSheet("background-color: rgba(255, 255, 255, 0);");

    if (today.year() == selectedYear && today.month() == selectedMonth && date == today.day())
        setCurrentDateFormat(lineEdit);

    lineEdit->setAttribute(Qt::WA_TransparentForMouseEvents);   // lets clicks pass to actual cell beneath

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
        if (event == "" && date.isValid())
            FileManager::updateFileTracker(selfPath, date.toString("dd/MM/yyyy"), "");  // cleanup dates of deleted events
        if (event != "")
            addToEventList(date, event);
    }

    file.close();
}

void Calendar::addToDateCell(QDate date, QWidget *textField)
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
            CalendarEvent *eventWidget = new CalendarEvent(this, date, eventName);
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

/**
 * @brief Calendar::newEventPrompt shows a new event prompt at a particular cell
 */
void Calendar::newEventPrompt(int row, int column)
{
    QDate date = getDateFromCell(row, column);
    if (!date.isValid())    return; // non-date cell

    this->cellWidget(row, column)->setStyleSheet("* {background: #4D4D4D; border-radius: 10px}");

    CalendarEvent *newEvent = new CalendarEvent(this, date, "");

    EventDialog *eventDialog = new EventDialog();
    QPoint position = this->cellWidget(row, column)->pos();
    position = this->mapToGlobal(position);
    eventDialog->displayDialog(newEvent, position);
    connect(eventDialog, &EventDialog::hidden, [=] { clearSelectedCell(this->cellWidget(row, column)); });
}

/**
 * @brief Calendar::clearSelectedCell removes cell selection formatting
 */
void Calendar::clearSelectedCell(QWidget *cellWidget)
{
    cellWidget->setStyleSheet("*:hover {background: #3E3E3E; border-radius: 10px}");
}

/**
 * @brief Calendar::cleanupReminders removes any reminders for current calendar
 */
void Calendar::cleanupReminders()
{
    QString newData = "";
    QString remindersStorage = QCoreApplication::applicationDirPath() + "/reminders.dat";
    QFile file(remindersStorage);
    file.open(QIODevice::ReadOnly);
    QTextStream data(&file);
    while (!data.atEnd())
    {
        QDateTime dateTime = QDateTime::fromString(data.readLine());
        QString eventPath = data.readLine();
        if (!eventPath.startsWith(selfPath.section("/", 0, -2)))
            newData.append(dateTime.toString() + "\n" + eventPath + "\n");
    }
    file.close();
    FileManager::writeToFile(remindersStorage, newData);
    RemindersContainer::refreshReminderList();
}
