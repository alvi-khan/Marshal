#include "sidebarmanager.h"
#include "error.h"
#include "filedisplay.h"

QTreeView * sidebarManager::sidebar;
QStandardItem * sidebarManager::temporaryPage;
bool sidebarManager::tempPageExists;

sidebarManager::sidebarManager()
{

}

QStandardItem* sidebarManager::addChild(QString fileName, QString filePath, QStandardItem *parent)
{
    QStandardItem* childItem = new QStandardItem(fileName);

    QList<QStandardItem *> data;
    data.append(childItem);
    data.append(new QStandardItem(filePath));

    parent->appendRow(data);
    return childItem;
}

void sidebarManager::removeItem(QModelIndex index)
{
    if (index == temporaryPage->index() && !tempPageExists) return;
    else if (index == temporaryPage->index())   tempPageExists = false;

    if (index.data().toString() == "")  return;
    QStandardItemModel *model = (QStandardItemModel *) sidebar->model();

    QStandardItem *temporaryItem = model->itemFromIndex(index);
    model->removeRow(temporaryItem->row(), temporaryPage->index().parent());
}

void sidebarManager::createEmptyPage()
{
    if (tempPageExists) return;

    QStandardItemModel *model = (QStandardItemModel *) sidebar->model();
    QModelIndex emptyPageParentIndex = sidebar->currentIndex();
    QString filePath = emptyPageParentIndex.siblingAtColumn(1).data().toString();

    QStandardItem *parent;
    if (filePath == "") parent = model->invisibleRootItem();
    else                parent = model->itemFromIndex(emptyPageParentIndex);

    temporaryPage = addChild("Untitled Page", "", parent);

    sidebar->setCurrentIndex(QModelIndex(temporaryPage->index()));

    tempPageExists = true;
    FileDisplay::lastIndex = sidebar->currentIndex();
}

void sidebarManager::init(QTreeView *sidebar)
{
    sidebarManager::sidebar = sidebar;
    tempPageExists = false;
    temporaryPage = new QStandardItem();
}
