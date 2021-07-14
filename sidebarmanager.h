#ifndef SIDEBARMANAGER_H
#define SIDEBARMANAGER_H

#include <QStandardItemModel>
#include <QTreeView>
#include <QItemSelection>

class sidebarManager
{
public:
    sidebarManager();
    static QStandardItem* addChild(QString fileName, QString filePath, QStandardItem *parent);
    static void removeItem(QModelIndex index = temporaryPage->index());
    static void createEmptyPage();
    static QTreeView *sidebar;
    static QStandardItem *temporaryPage;
    static bool tempPageExists;
    static void init(QTreeView *sidebar);
};

#endif // SIDEBARMANAGER_H
