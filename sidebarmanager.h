#ifndef SIDEBARMANAGER_H
#define SIDEBARMANAGER_H

#include <QString>
#include <QStandardItem>
#include <QTreeView>

class SidebarManager
{
public:
    SidebarManager();
    static QStandardItem* createItem(QString fileName, QString filePath, QStandardItem *parent = nullptr);
    static void getChildren(QString directory, QStandardItem *parent);
    static void init(QTreeView *sidebar);
    static void rename(QModelIndex index, QString newName);
    static QString homeDirectory;
    static QTreeView *sidebar;
    void removeItem(QModelIndex index);
    static QModelIndex getChild(QString data);

    static void setCurrentIndex(QModelIndex index);
    static QModelIndex getCurrentIndex();
    static QStandardItem* getItemAt(QModelIndex index);
};

#endif // SIDEBARMANAGER_H
