#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "filemanager.h"
#include <QDir>
#include <QFileDialog>
#include <QGraphicsBlurEffect>
#include "databasemanager.h"
#include "mainwindow.h"
#include "sidebarmanager.h"


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowIcon(QIcon(":/Icons/Resources/Icons/Settings.svg"));
    this->setWindowTitle("Settings");
    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect();
    blurEffect->setBlurRadius(10);
    MainWindow::window->setGraphicsEffect(blurEffect);

    this->ui->directoryTextField->setText(FileManager::homeDirectory.section("/", 0, -3));
    oldDir = FileManager::homeDirectory.section("/", 0, -2);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_browseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), oldDir.section("/", 0, -2), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir != "") this->ui->directoryTextField->setText(dir);
}


void SettingsDialog::on_buttonBox_accepted()
{
    QString directory = this->ui->directoryTextField->text();
    if (directory.endsWith("/")) directory.chop(1);

    // update variables everywhere
    FileManager::homeDirectory = directory + "/Marshal User Files/Private";
    SidebarManager::homeDirectory = FileManager::homeDirectory;
    SidebarManager::sharedDirectory = directory + "/Marshal User Files/Shared";
    DatabaseManager::homeDirectory = FileManager::homeDirectory.section("/", 0, -2);
    DatabaseManager::privateDirectory = DatabaseManager::homeDirectory + "/Private";
    DatabaseManager::sharedDirectory = DatabaseManager::homeDirectory + "/Shared";

    // update settings configuration file
    QString settingsFilePath = QCoreApplication::applicationDirPath() + "/settings.conf";
    FileManager::writeToFile(settingsFilePath, FileManager::homeDirectory);

    // move content to new directory
    QString newDir = directory + "/Marshal User Files";
    QDir dir;
    dir.mkpath(directory);
    dir.rename(oldDir, newDir);

    // close dialog
    MainWindow::window->setGraphicsEffect(nullptr);
    accept();
}

void SettingsDialog::on_buttonBox_rejected()
{
    MainWindow::window->setGraphicsEffect(nullptr);
    reject();
}

void SettingsDialog::closeEvent(QCloseEvent *event)
{
    MainWindow::window->setGraphicsEffect(nullptr);
    this->accept();
}

