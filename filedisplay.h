#ifndef FILEDISPLAY_H
#define FILEDISPLAY_H

#include<QStandardItemModel>
#include<QTextBrowser>
#include<QFile>
#include<QLineEdit>
#include<QDir>
#include<QTreeView>

class FileDisplay
{
public:
    FileDisplay();
    static void openFile(QModelIndex index);
    static void saveFile();
    static void changeTitle();
    static void init(QTextBrowser *mainPage, QLineEdit *pageTitle, QTreeView *sideBar);
private:
    static QString fileExtension;
    static QTextBrowser *mainPage;
    static QLineEdit *pageTitle;
    static QTreeView *sideBar;
};

#endif // FILEDISPLAY_H
