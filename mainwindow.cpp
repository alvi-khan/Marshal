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
}

void MainWindow::on_newPageButton_clicked()
{
    sidebarManager::createEmptyPage();
    this->ui->mainPage->setText("");
    this->ui->pageTitle->setText("");

}


void MainWindow::on_mainPage_textChanged()
{
    //if (this->ui->mainPage->toPlainText() == "")    this->ui->mainPage->setTextColor(QColor(Qt::gray));
    //else    this->ui->mainPage->setTextColor(QColor(Qt::white));
    display->saveFile();
    // TODO show placeholder text if empty
}


void MainWindow::on_pageTitle_textChanged()
{
    // TODO show placehoder text again
}


void MainWindow::on_pageTitle_editingFinished()
{
    display->changeTitle();
}
