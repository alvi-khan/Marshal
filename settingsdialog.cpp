#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "filemanager.h"
#include <QDir>
#include <QFileDialog>


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->ui->directoryTextField->setText(FileManager::homeDirectory.section("/", 0, -3));
    oldDir = FileManager::homeDirectory.section("/", 0, -2);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_browseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), oldDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir != "") this->ui->directoryTextField->setText(dir);
}


void SettingsDialog::on_buttonBox_accepted()
{
    QString directory = this->ui->directoryTextField->text();
    FileManager::homeDirectory = directory + "/Marshal User Files/Private";
    QString settingsFilePath = QCoreApplication::applicationDirPath() + "/settings.conf";
    FileManager::writeToFile(settingsFilePath, FileManager::homeDirectory);
    QString newDir = directory + "/Marshal User Files";
    QDir dir;
    dir.mkpath(directory);
    dir.rename(oldDir, newDir);
}

