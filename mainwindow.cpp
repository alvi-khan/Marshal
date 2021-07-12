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
    display->openFile(index, this->ui->mainPage, this->ui->pageTitle);
}

void MainWindow::init()
{
    fileManager = new FileManager();
    fileManager->init(this->ui->sideBar);

    display = new FileDisplay();

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
    display->saveFile(index, this->ui->mainPage);
}


void MainWindow::on_pageTitle_textChanged()
{
    QStandardItemModel *model = (QStandardItemModel *) this->ui->sideBar->model();
    QModelIndex index = this->ui->sideBar->currentIndex();
    QLineEdit *newFileName = this->ui->pageTitle;

    display->changeTitle(model, index, newFileName);
}

