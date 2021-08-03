#ifndef SIDEBARMANAGER_H
#define SIDEBARMANAGER_H

#include <QString>
#include <QStandardItem>
#include <QTreeView>

/**
 * @brief The SidebarManager class is a utility class that manages the sidebar of the program.
 */
class SidebarManager
{
public:
    SidebarManager();
    static QStandardItem* createItem(QString fileName, QString filePath, QStandardItem *parent = nullptr);
    static void addChildren(QString directory, QStandardItem *parent);
    static void init(QTreeView *sidebar);
    static void rename(QModelIndex index, QString newName);
    static QString homeDirectory;
    static QTreeView *sidebar;
    static QStandardItemModel *model;
    void removeItem(QModelIndex index);
    static QModelIndex getChild(QString data);

    static void setCurrentIndex(QModelIndex index);
    static QModelIndex getCurrentIndex();
    static QStandardItem* getItemAt(QModelIndex index);
    static void reloadSidebar();
};

#endif // SIDEBARMANAGER_H
