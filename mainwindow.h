#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sideBar_clicked(const QModelIndex &index);

    void on_newPageButton_clicked();

    void on_mainPage_textChanged();

    void on_pageTitle_textChanged();

private:
    Ui::MainWindow *ui;
    FileManager *fileManager;
    void init();
};
#endif // MAINWINDOW_H