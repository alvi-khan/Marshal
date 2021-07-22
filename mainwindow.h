#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_pageTitle_editingFinished();

    void on_urlButton_clicked();

    void on_subpageButton_clicked();

private:
    Ui::MainWindow *ui;
    void init();
};
#endif // MAINWINDOW_H
