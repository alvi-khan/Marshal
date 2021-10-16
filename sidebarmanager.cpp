#include "sidebarmanager.h"

#include <QDirIterator>
#include <QSortFilterProxyModel>
#include <QStack>
#include "filemanager.h"

QString SidebarManager::homeDirectory;
QString SidebarManager::sharedDirectory;
QTreeView * SidebarManager::sidebar;
QStandardItemModel * SidebarManager::model;

SidebarManager::SidebarManager()
{

}

/**
 * @brief SidebarManager::getChild retrieves index of child page with specific path
 * @param path is the path to match
 * @return the index of the child page
 */
QModelIndex SidebarManager::getChild(QString path)    // opens sub file
{
    // get current parent
    QStandardItem *parent = model->itemFromIndex(sidebar->currentIndex());
    QStandardItem *child;
    QString childPath;

    // for each child of current parent
    for (int row=0; row < parent->rowCount(); row++)
    {
        child = parent->child(row, 0);
        childPath = FileManager::homeDirectory + child->index().siblingAtColumn(1).data().toString();
        // if child has require path, return its index
        if (QString::compare(childPath, path, Qt::CaseInsensitive) == 0)
            return child->index();
    }
    // if child not found, return parent's index
    return sidebar->currentIndex();
}

void SidebarManager::removeItem(QModelIndex index)
{
    if (!index.isValid())  return;

    model->removeRow(index.row(), index.parent());
}

QStandardItem* SidebarManager::createItem(QString fileName, QString filePath, QStandardItem *parent)
{
    // TODO this condition may be unnecessary; verify and remove
    if (parent == nullptr)
    {
        if (!sidebar->currentIndex().isValid()) parent = model->invisibleRootItem();
        else    parent = model->itemFromIndex(sidebar->currentIndex());
    }

    QStandardItem* childItem = new QStandardItem(fileName);

    QList<QStandardItem *> data;
    data.append(childItem);
    data.append(new QStandardItem(filePath.remove(FileManager::homeDirectory)));

    parent->appendRow(data);
    return childItem;
}

/**
 * @brief SidebarManager::addChildren adds all child items for a specific parent to the sidebar
 * @param directory is the file path to the parent
 * @param parent is the parent item
 */
void SidebarManager::addChildren(QString directory, QStandardItem *parent)
{
    // loop through every subdirectory under the parent directory
    QDirIterator it(directory, QDir::Dirs|QDir::NoDotAndDotDot);
    while (it.hasNext())
    {
        QDir dir = it.next();
        if (!QFile::exists(dir.path() + "/files.mar"))  continue;
        // create a new item for every subdirectory (subpage)
        QStandardItem* newItem = createItem(dir.dirName(), dir.absolutePath(), parent);
        // add children for each subdirectory (subpage)
        addChildren(dir.absolutePath(), newItem);
    }
}

/**
 * @brief SidebarManager::init populates the sidebar
 * @param sidebar
 */
void SidebarManager::init(QTreeView *sidebar)
{
    homeDirectory = FileManager::homeDirectory;
    sharedDirectory = homeDirectory.section("/", 0, -2) + "/Shared";
    SidebarManager::sidebar = sidebar;
    model = new QStandardItemModel();
    addChildren(homeDirectory, model->invisibleRootItem());
    addChildren(sharedDirectory, model->invisibleRootItem());
    sidebar->setModel(model);
    sidebar->setColumnHidden(1, true);
    sidebar->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sidebar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(sidebar, SIGNAL(customContextMenuRequested(const QPoint &)), new SidebarManager(), SLOT(onCustomContextMenu(const QPoint &)));
}

void SidebarManager::onCustomContextMenu(const QPoint &point)
{
    QModelIndex index = sidebar->indexAt(point);
    if (!index.isValid())   return;

    QMenu menu;
    menu.setStyleSheet("QMenu{background-color: rgb(46, 46, 46); color: rgb(255, 255, 255); padding: 2px;}\
                        QMenu::item:selected{background-color: #3E3E3E; color: rgb(255, 255, 255); border-radius: 2px;}");

    QAction* del = new QAction("Delete", this);
    del->setIcon(QIcon(":/Toolbar Icons/Resources/Toolbar Icons/Trash (Context Menu).svg"));

    connect(del, &QAction::triggered, [=] { FileManager::deletePage(FileManager::homeDirectory + index.siblingAtColumn(1).data().toString()); });
    menu.addAction(del);

    menu.exec(QCursor::pos());
    del->deleteLater();
}

void SidebarManager::reloadSidebar()
{
    model->removeRows(0, model->rowCount());
    addChildren(homeDirectory, model->invisibleRootItem());
    addChildren(sharedDirectory, model->invisibleRootItem());
    sidebar->setColumnHidden(1, true);
}

void SidebarManager::rename(QModelIndex index, QString newName)
{
    // update item name
    QStandardItem *item = model->itemFromIndex(index);
    item->setText(newName);

    // update item filepath
    item = model->itemFromIndex(index.siblingAtColumn(1));
    QString newPath = item->text();
    newPath.truncate(newPath.lastIndexOf(QChar('/')));
    newPath += "/" + newName;
    item->setText(newPath);
}

void SidebarManager::setCurrentIndex(QModelIndex index)
{
    sidebar->setCurrentIndex(index);
}

QModelIndex SidebarManager::getCurrentIndex()
{
    return sidebar->currentIndex();
}

QStandardItem* SidebarManager::getItemAt(QModelIndex index)
{
    if (index.isValid())    return model->itemFromIndex(index);
    else                    return model->invisibleRootItem();
}

QList<QModelIndex> SidebarManager::getItemList(QAbstractItemModel *model)
{
    QList<QModelIndex> results;
    QStack<QModelIndex> unprocessedItems;
    for (int i=0; i<model->rowCount(); i++)
        unprocessedItems.push(model->index(i, 0));
    while(!unprocessedItems.empty())
    {
        QModelIndex index = unprocessedItems.pop();
        results.push_back(index);
        for (int i=0; i<model->rowCount(index); i++)
            unprocessedItems.push(model->index(i, 0, index));
    }
    return results;
}

void SidebarManager::filterItems(QString filter)
{
    QList<QModelIndex> items = getItemList(model);
    foreach (QModelIndex index, items)
        sidebar->setRowHidden(index.row(), index.parent(), true);

    QSortFilterProxyModel *filteredModel = new QSortFilterProxyModel();
    filteredModel->setSourceModel(model);
    filteredModel->setRecursiveFilteringEnabled(true);
    filteredModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filteredModel->setFilterFixedString(filter);

    items = getItemList(filteredModel);
    foreach (QModelIndex index, items)
    {
        QModelIndex sourceIndex = filteredModel->mapToSource(index);
        sidebar->setRowHidden(sourceIndex.row(), sourceIndex.parent(), false);

        sourceIndex = sourceIndex.parent();
        while(sourceIndex.isValid())
        {
            sidebar->expand(sourceIndex);
            sourceIndex = sourceIndex.parent();
        }
    }
}
