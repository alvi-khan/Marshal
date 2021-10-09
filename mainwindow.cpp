#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sidebarmanager.h"
#include "displaymanager.h"
#include "filemanager.h"
#include "blocks.h"
#include "mainpage.h"
#include "calendar.h"
#include <richtextfunctions.h>
#include "handleexternalfile.h"
#include "databasemanager.h"
#include "logindialog.h"
#include "settingsdialog.h"

MainWindow * MainWindow::window;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    window = this;
    init();
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::init()
{
    QString settingsFilePath = QCoreApplication::applicationDirPath() + "/settings.conf";
    FileManager::homeDirectory = FileManager::readFromFile(settingsFilePath);

    // initialize utility classes
    //DatabaseManager::init();
    SidebarManager::init(this->ui->sideBar);
    DisplayManager::init(this->ui->mainPage, this->ui->pageTitle, this->ui->filePath);
    Blocks::init(this->ui->mainPage);
    RichTextFunctions::init(this->ui->boldButton, this->ui->italicButton, this->ui->underlineButton);

    // set up default splitter size
    QSplitter *splitter = this->ui->splitter;
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    // TODO hide from designer before final release
    this->ui->pageTitle->setVisible(false);
    this->ui->mainPage->setVisible(false);
    this->ui->toolbar->setVisible(false);

    this->ui->sideBar->setStyleSheet(""""
        "QTreeView::branch:open:has-children:!has-siblings,"
        "QTreeView::branch:open:has-children:has-siblings"
        "{"
            "border-image: none;"
            "image: url(:/Icons/Resources/Icons/Drop Down Open.svg);"
        "}"
        "QTreeView::branch:has-children:!has-siblings:closed,"
        "QTreeView::branch:closed:has-children:has-siblings"
        "{"
            "border-image: none;"
            "image: url(:/Icons/Resources/Icons/Drop Down Closed.svg);"
        "}"
    """");

    connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(onFocusChange(QWidget*, QWidget*)));

    this->ui->mainPage->layout()->setAlignment(Qt::AlignHCenter);

    this->ui->sideBar->setColumnHidden(1, true);
}

void MainWindow::onFocusChange(QWidget *oldWidget, QWidget *newWidget)
{
    if (QApplication::activeWindow() == nullptr)
        return;

    QString objName = newWidget->objectName();
    QTextBrowser *block = qobject_cast<QTextBrowser*>(oldWidget);

    if (block != nullptr  && objName == "boldButton")
        RichTextFunctions::boldText(block);

    else if (block != nullptr  && objName == "italicButton")
        RichTextFunctions::italicText(block);

    else if (block != nullptr  && objName == "underlineButton")
        RichTextFunctions::underlineText(block);
}

/**
 * @brief MainWindow::revealMainPage makes the main page visible
 */
void MainWindow::revealMainPage()
{
    this->ui->pageTitle->setVisible(true);
    this->ui->mainPage->setVisible(true);
    this->ui->toolbar->setVisible(true);
}

/**
 * @brief MainWindow::on_sideBar_clicked, open the clicked file
 * @param index is the index of the file clicked
 */
void MainWindow::on_sideBar_clicked(const QModelIndex &index)
{
    revealMainPage();
    while (QWidget *w = this->ui->mainPage->findChild<QWidget*>())  delete w;   // delete all hidden objects; memory leak avoidance
    DisplayManager::openFile(index);
}

/**
 * @brief MainWindow::on_newPageButton_clicked creates a new root level page
 */
void MainWindow::on_newPageButton_clicked()        // creates a new page
{
    revealMainPage();
    this->ui->searchBox->setText("");
    QStandardItemModel *model = (QStandardItemModel*) this->ui->sideBar->model();
    FileManager::addFile(model->invisibleRootItem()->index());
    this->ui->sideBar->hideColumn(1);
}

/**
 * @brief MainWindow::on_pageTitle_editingFinished updates the page name
 */
void MainWindow::on_pageTitle_editingFinished()
{
    DisplayManager::renameFile(this->ui->sideBar->currentIndex());
}

/**
 * @brief MainWindow::on_urlButton_clicked creates a new URL block
 */
void MainWindow::on_urlButton_clicked()
{
    //DisplayManager::createUrl("www.google.com", "Google");
}

/**
 * @brief MainWindow::on_subpageButton_clicked creates a new subpage under the current page
 */
void MainWindow::on_subpageButton_clicked()
{
    // TODO first line should be unnecessary in final release; subpage cannot be created without page already open
    revealMainPage();
    FileManager::addFile(this->ui->sideBar->currentIndex());
}

void MainWindow::on_calendar_clicked()
{
    FileManager::addCalendar(this->ui->sideBar->currentIndex());
}



void MainWindow::on_externalFileButton_clicked()
{
     HandleExternalFile::addExternalFile();
}

void MainWindow::on_profileButton_clicked()
{
    LoginDialog *loginDialog = new LoginDialog();
    loginDialog->exec();
    delete loginDialog;
}


void MainWindow::on_shareButton_clicked()
{
    DatabaseManager::shareFile();
}


void MainWindow::on_searchBox_textChanged(const QString &arg1)
{
    SidebarManager::filterItems(arg1);
}


void MainWindow::on_settingsButton_clicked()
{
    SettingsDialog *settingsDialog = new SettingsDialog();
    //settingsDialog->setWindowIcon(QIcon(":/Icons/Resources/Icons/Profile.svg"));
    settingsDialog->setWindowTitle("Settings");
    settingsDialog->exec();
    delete settingsDialog;
}

