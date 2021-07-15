#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    this->ui->pageTitle->setVisible(true);
    this->ui->mainPage->setVisible(true);
    display->openFile(index);
}

void MainWindow::init()
{
    FileManager::init(this->ui->mainPage, this->ui->sideBar);
    FileDisplay::init(this->ui->mainPage, this->ui->pageTitle, this->ui->sideBar);
    sidebarManager::init(this->ui->sideBar);

    QSplitter *splitter = this->ui->splitter;
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    QPalette *palette = new QPalette();
    palette->setColor(QPalette::PlaceholderText, Qt::darkGray);
    this->ui->pageTitle->setPalette(*palette);
    this->ui->mainPage->setPalette(*palette);

    this->ui->pageTitle->setVisible(false);
    this->ui->mainPage->setVisible(false);
}

void MainWindow::on_newPageButton_clicked()
{
    this->ui->pageTitle->setVisible(true);
    this->ui->mainPage->setVisible(true);
    this->ui->mainPage->setText("");
    this->ui->pageTitle->setText("");
    sidebarManager::createEmptyPage();
}


void MainWindow::on_mainPage_textChanged()
{
    display->saveFile();
}


void MainWindow::on_pageTitle_editingFinished()
{
    display->changeTitle();
}
