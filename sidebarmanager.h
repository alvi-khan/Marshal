#ifndef SIDEBARMANAGER_H
#define SIDEBARMANAGER_H

#include <QString>
#include <QObject>
#include <QStandardItem>
#include <QTreeView>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>

/**
 * @brief The SidebarManager class is a utility class that manages the sidebar of the program.
 */
class SidebarManager : public QObject
{
    Q_OBJECT
public:
    SidebarManager();
    static QStandardItem* createItem(QString fileName, QString filePath, QStandardItem *parent = nullptr);
    static void addChildren(QString directory, QStandardItem *parent);
    static void init(QTreeView *sidebar);
    static void rename(QModelIndex index, QString newName);
    static QString homeDirectory;
    static QString sharedDirectory;
    static QTreeView *sidebar;
    static QStandardItemModel *model;
    static void removeItem(QModelIndex index);
    static QModelIndex getChild(QString data);

    static void setCurrentIndex(QModelIndex index);
    static QModelIndex getCurrentIndex();
    static QStandardItem* getItemAt(QModelIndex index);
    static void reloadSidebar();
    static void filterItems(QString filter);
private:

    static QList<QModelIndex> getItemList(QAbstractItemModel *model);
public slots:
    void onCustomContextMenu(const QPoint &point);
};

#endif // SIDEBARMANAGER_H
