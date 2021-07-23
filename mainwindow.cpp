#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sidebarmanager.h"
#include "displaymanager.h"
#include "filemanager.h"
#include "blocks.h"
#include "mainpage.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::init()
{
    SidebarManager::init(this->ui->sideBar);
    DisplayManager::init(this->ui->mainPage, this->ui->pageTitle);
    Blocks::init(this->ui->mainPage);

    QSplitter *splitter = this->ui->splitter;
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    this->ui->pageTitle->setVisible(false);
    this->ui->mainPage->setVisible(false);
}


void MainWindow::on_sideBar_clicked(const QModelIndex &index)     // opens a file from sidebar
{
    this->ui->pageTitle->setVisible(true);
    this->ui->mainPage->setVisible(true);
    DisplayManager::openFile(index);
}


void MainWindow::on_newPageButton_clicked()        // creates a new page
{
    this->ui->pageTitle->setVisible(true);
    this->ui->mainPage->setVisible(true);
    QStandardItemModel *model = (QStandardItemModel*) this->ui->sideBar->model();
    FileManager::addFile(model->invisibleRootItem()->index());
}

void MainWindow::on_pageTitle_editingFinished()     // changes page title
{
    DisplayManager::renameFile(this->ui->sideBar->currentIndex());
}


void MainWindow::on_urlButton_clicked()
{
    DisplayManager::createUrl("www.google.com", "Google");
}

void MainWindow::on_subpageButton_clicked()
{
    this->ui->pageTitle->setVisible(true);
    this->ui->mainPage->setVisible(true);
    FileManager::addFile(this->ui->sideBar->currentIndex());
}

