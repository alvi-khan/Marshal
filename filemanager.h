#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QTreeView>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QTextBrowser>
#include "sidebarmanager.h"

class FileManager
{
public:
    FileManager();
    static void init(QTextBrowser *mainPage, QTreeView *sidebar);
    static void addPage(QString fileName);
private:
    static QString homeDirectory;
    static QString fileExtension;
    static QTreeView *sidebar;
    static void getChildren(QString directory, QStandardItem *parent);
    static QTextBrowser *mainPage;
};

#endif // FILEMANAGER_H
