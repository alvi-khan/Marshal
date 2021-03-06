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
#include "reminderscontainer.h"

#include <QMovie>

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
    if(!QFile::exists(settingsFilePath))
            FileManager::writeToFile(settingsFilePath, QCoreApplication::applicationDirPath() + "/Marshal User Files/Private");
    FileManager::homeDirectory = FileManager::readFromFile(settingsFilePath);

    loadingGIF = new QMovie(":/Icons/Resources/Icons/Loading.gif");
    connect(loadingGIF, &QMovie::frameChanged, [=]{this->ui->profileButton->setIcon(loadingGIF->currentPixmap());});

    mainPage = this->ui->mainPage;

    RemindersContainer::retrieveReminders();

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

    this->ui->mainPageFrame->setVisible(false);

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
    this->ui->mainPageFrame->setVisible(true);
}

void MainWindow::hideMainPage()
{
    this->ui->mainPageFrame->setVisible(false);
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
    // TODO implement URL attachment
}

/**
 * @brief MainWindow::on_subpageButton_clicked creates a new subpage under the current page
 */
void MainWindow::on_subpageButton_clicked()
{
    // TODO first line should be unnecessary; subpage cannot be created without page already open
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
}


void MainWindow::on_shareButton_clicked()
{
    DatabaseManager::shareFile();
}


void MainWindow::on_searchBox_textChanged(const QString &arg1)
{
    SidebarManager::filterItems(arg1);
}


void MainWindow::on_trashButton_clicked()
{
    FileManager::deletePage(FileManager::openFile);
}

void MainWindow::on_settingsButton_clicked()
{
    SettingsDialog *settingsDialog = new SettingsDialog();
    settingsDialog->exec();
    delete settingsDialog;
}

void MainWindow::toggleLoadingGIF()
{
    if (loadingGIF->state() == QMovie::Running)
    {
        loadingGIF->stop();
        this->ui->profileButton->setIcon(QIcon(":/Icons/Resources/Icons/Profile.svg"));
    }
    else    loadingGIF->start();
}


void MainWindow::on_remindersButton_clicked()
{
    RemindersContainer *reminders = new RemindersContainer();
    reminders->exec();
}

