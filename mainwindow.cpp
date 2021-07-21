#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sidebarmanager.h"
#include "displaymanager.h"
#include "filemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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

    QSplitter *splitter = this->ui->splitter;
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    this->ui->pageTitle->setVisible(false);
    this->ui->mainPage->setVisible(false);
}


void MainWindow::on_sideBar_clicked(const QModelIndex &index)
{
    this->ui->pageTitle->setVisible(true);
    this->ui->mainPage->setVisible(true);
    DisplayManager::openFile(index);
}


void MainWindow::on_newPageButton_clicked()
{
    this->ui->pageTitle->setVisible(true);
    this->ui->mainPage->setVisible(true);
    FileManager::addFile(this->ui->sideBar->currentIndex());
}

void MainWindow::on_pageTitle_editingFinished()
{
    DisplayManager::renameFile(this->ui->sideBar->currentIndex());
}

