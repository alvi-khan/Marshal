#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainpage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    static MainWindow *window;
    ~MainWindow();

private slots:
    void on_sideBar_clicked(const QModelIndex &index);

    void on_newPageButton_clicked();

    void on_pageTitle_editingFinished();

    void on_urlButton_clicked();

    void on_subpageButton_clicked();


    void onFocusChange(QWidget *oldWidget, QWidget *newWidget);

    void on_calendar_clicked();

    void on_externalFileButton_clicked();

    void on_profileButton_clicked();

    void on_shareButton_clicked();

    void on_searchBox_textChanged(const QString &arg1);

    void on_trashButton_clicked();
    
    void on_settingsButton_clicked();

    void on_remindersButton_clicked();

private:
    void init();
    void revealMainPage();
    QMovie *loadingGIF;

public:
    Ui::MainWindow *ui;
    void hideMainPage();
    void toggleLoadingGIF();
    MainPage *mainPage;
};
#endif // MAINWINDOW_H
