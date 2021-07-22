#include "sidebarmanager.h"

#include <QDirIterator>

QString SidebarManager::homeDirectory;
QTreeView * SidebarManager::sidebar;

SidebarManager::SidebarManager()
{

}

QModelIndex SidebarManager::getChild(QString data)    // opens sub file
{
    QStandardItemModel *model = (QStandardItemModel *) sidebar->model();
    QStandardItem *parent = model->itemFromIndex(sidebar->currentIndex());
    QStandardItem *child;

    for (int row=0; row < parent->rowCount(); row++)
    {
        child = parent->child(row, 0);
        if (QString::compare(child->index().siblingAtColumn(1).data().toString(), data, Qt::CaseInsensitive) == 0)
            return child->index();
    }
    return sidebar->currentIndex();
}

void SidebarManager::removeItem(QModelIndex index)
{
    if (!index.isValid())  return;
    QStandardItemModel *model = (QStandardItemModel *) sidebar->model();

    QStandardItem *item = model->itemFromIndex(index);
    model->removeRow(item->row(), item->parent()->index());
}

/**
 * @brief SidebarManager::createItem creates a new entry in the sidebar
 * @param fileName
 * @param filePath
 * @param parent
 * @return
 */
QStandardItem* SidebarManager::createItem(QString fileName, QString filePath, QStandardItem *parent)
{
    if (parent == nullptr)
    {
        QStandardItemModel *model = (QStandardItemModel *) sidebar->model();
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
 * @brief SidebarManager::getChildren retrieves all child items from a particular location
 * @param directory
 * @param parent
 */
void SidebarManager::getChildren(QString directory, QStandardItem *parent)
{
    QDirIterator it(directory, QDir::Dirs|QDir::NoDotAndDotDot);
    while (it.hasNext())
    {
        QDir dir = it.next();
        QStandardItem* newItem = createItem(dir.dirName(), dir.absolutePath(), parent);
        getChildren(dir.absolutePath(), newItem);
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
    QStandardItemModel *model = new QStandardItemModel();
    getChildren(homeDirectory, model->invisibleRootItem());
    sidebar->setModel(model);
    //sidebar->setColumnHidden(1, true);
}

void SidebarManager::rename(QModelIndex index, QString newName)
{
    QStandardItemModel *model = (QStandardItemModel *) sidebar->model();
    QStandardItem *item = model->itemFromIndex(index);
    item->setText(newName);
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
    QStandardItemModel *model = (QStandardItemModel *) sidebar->model();
    if (index.isValid())    return model->itemFromIndex(index);
    else                    return model->invisibleRootItem();
}
