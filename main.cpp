#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":/Icons/Resources/Icons/Marshal Logo.svg"));
    w.setWindowTitle("Marshal");
    w.show();

    return a.exec();
}
