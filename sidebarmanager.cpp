#include "sidebarmanager.h"

#include <QDirIterator>

QString SidebarManager::homeDirectory;
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
        childPath = child->index().siblingAtColumn(1).data().toString();
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

    QStandardItem *item = model->itemFromIndex(index);
    model->removeRow(item->row(), item->parent()->index());
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
    data.append(new QStandardItem(filePath));

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
    homeDirectory = "E:/Downloads/Main Folder";
    SidebarManager::sidebar = sidebar;
    model = new QStandardItemModel();
    addChildren(homeDirectory, model->invisibleRootItem());
    sidebar->setModel(model);
    sidebar->setColumnHidden(1, true);
}

void SidebarManager::reloadSidebar()
{
    model->removeRows(0, model->rowCount());
    addChildren(homeDirectory, model->invisibleRootItem());
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
