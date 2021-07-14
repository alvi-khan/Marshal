#include "filemanager.h"
#include "sidebarmanager.h"

QTreeView * FileManager::sidebar;
QString FileManager::homeDirectory;
QString FileManager::fileExtension;
QTextBrowser * FileManager::mainPage;

FileManager::FileManager()
{

}

/**
 * @brief FileManager::createNewPage adds a new page to the sidebar
 */
void FileManager::addPage(QString fileName)
{
    QTreeView* sidebar = sidebarManager::sidebar;
    QStandardItemModel *model = (QStandardItemModel *) sidebar->model();
    QModelIndex selectedIndex = sidebar->currentIndex();
    QString filePath = selectedIndex.parent().siblingAtColumn(1).data().toString();

    QStandardItem *parent;

    if (filePath == "")
    {
        filePath = homeDirectory;
        parent = model->invisibleRootItem();
    }
    else    parent = model->itemFromIndex(selectedIndex.parent());

    filePath += "\\" + fileName;

    QDir dir;
    dir.mkpath(filePath);
    QFile file(filePath + "\\" + fileName + fileExtension);
    file.open(QFile::ReadWrite);
    QTextStream text(&file);
    text << mainPage->document()->toHtml();

    file.close();

    QStandardItem *finalPage = sidebarManager::addChild(fileName, filePath, parent);
    sidebar->setCurrentIndex(QModelIndex(finalPage->index()));
    sidebarManager::removeItem(selectedIndex);
    sidebar->setExpanded(selectedIndex, true);
    sidebar->setColumnHidden(1, true);
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
            sidebarManager sidebarManager;
            QStandardItem* item = sidebarManager.addChild(fileName, directory + "\\" + fileName, parent);
            getChildren(directory + "\\" + fileName, item);
        }
    }
}

void FileManager::init(QTextBrowser *mainPage, QTreeView *sidebar)
{
    homeDirectory = "E:\\Downloads\\Main Folder";
    fileExtension = ".txt";
    FileManager::mainPage = mainPage;

    FileManager::sidebar = sidebar;
    QStandardItemModel *model = new QStandardItemModel();
    getChildren(homeDirectory, model->invisibleRootItem());
    sidebar->setModel(model);
    sidebar->setColumnHidden(1, true);
}
