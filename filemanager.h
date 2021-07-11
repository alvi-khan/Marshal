#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QTreeView>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QTextBrowser>

class FileManager
{
public:
    FileManager();
    void init(QTreeView *sidebar);
    void createNewPage();
private:
    QString homeDirectory;
    QString fileExtension;
    QTreeView *sidebar;
    void updateSidebar();
    QStandardItem* addItem(QString directory, QStandardItem *parent);
};

#endif // FILEMANAGER_H
