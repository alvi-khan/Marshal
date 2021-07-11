#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filedisplay.h"

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


void MainWindow::on_sideBar_clicked(const QModelIndex &index)
{
    FileDisplay *display = new FileDisplay();
    display->openFile(index, this->ui->mainPage, this->ui->pageTitle);
}

void MainWindow::init()
{
    fileManager = new FileManager();
    fileManager->init(this->ui->sideBar);

    QSplitter *splitter = this->ui->splitter;
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
}

void MainWindow::on_newPageButton_clicked()
{
    fileManager->createNewPage();
}


void MainWindow::on_mainPage_textChanged()
{
    QModelIndex index = this->ui->sideBar->currentIndex();
    FileDisplay *display = new FileDisplay();
    display->saveFile(index, this->ui->mainPage);
}


void MainWindow::on_pageTitle_textChanged()
{
    QModelIndex index = this->ui->sideBar->currentIndex();
    FileDisplay *display = new FileDisplay();
    display->changeTitle(index, this->ui->pageTitle);
}

