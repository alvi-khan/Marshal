#include "filemanager.h"

FileManager::FileManager()
{
    homeDirectory = "E:\\Downloads\\Main Folder";
    fileExtension = ".txt";
}

/**
 * @brief FileManager::addChild adds a child item to a parent item
 * @param fileName of the child item
 * @param filePath of the child item
 * @param parent
 * @return the child item created
 */
QStandardItem* FileManager::addChild(QString fileName, QString filePath, QStandardItem *parent)
{
    QStandardItem* item = new QStandardItem(fileName);
    QStandardItem* fullPath = new QStandardItem(filePath + "\\" + fileName + fileExtension);

    QList<QStandardItem *> data;
    data.append(item);
    data.append(fullPath);

    parent->appendRow(data);
    return item;
}

/**
 * @brief FileManager::createNewPage adds a new page to the sidebar
 */
void FileManager::createNewPage()
{
    QStandardItemModel *model = (QStandardItemModel *) sidebar->model();
    QModelIndex selectedIndex = sidebar->currentIndex();

    QString filePath = selectedIndex.siblingAtColumn(1).data().toString();
    QStandardItem *parent;

    if (filePath == "")
    {
        filePath = homeDirectory;
        parent = model->invisibleRootItem();
    }
    else
    {
        int cutOff = filePath.lastIndexOf(QChar('\\'));
        filePath = filePath.left(cutOff);
        parent = model->itemFromIndex(selectedIndex);
    }
    filePath += "\\Untitled Page";

    QDir dir;
    dir.mkpath(filePath);
    QFile file(filePath + "\\Untitled Page" + fileExtension);
    file.open(QFile::ReadWrite);
    file.close();

    addChild("Untitled Page", filePath, parent);
    sidebar->setExpanded(selectedIndex, true);
}

/**
 * @brief FileManager::getChildren retrieves all possible child items
 * @param directory of the parent item
 * @param parent
 */
void FileManager::getChildren(QString directory, QStandardItem *parent)
{
    QDirIterator it(directory, QDir::Dirs|QDir::NoDotAndDotDot);
    while (it.hasNext())
    {
        QStringList nameFilter("*" + fileExtension);
        QDir dir = it.next();
        bool validDir = !dir.entryList(nameFilter).empty();

        if (validDir)
        {
            QString fileName = dir.dirName().section("/", -1);
            QStandardItem* item = addChild(fileName, directory + "\\" + fileName, parent);
            getChildren(directory + "\\" + fileName, item);
        }
    }
}

void FileManager::init(QTreeView *sidebar)
{
    this->sidebar = sidebar;
    QStandardItemModel *model = new QStandardItemModel();
    getChildren(homeDirectory, model->invisibleRootItem());
    sidebar->setModel(model);
    sidebar->setColumnHidden(1, true);
}


