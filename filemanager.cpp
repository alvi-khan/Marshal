#include "filemanager.h"

FileManager::FileManager()
{
    homeDirectory = "E:\\Downloads\\Main Folder";
    fileExtension = ".txt";
}

QStandardItem* FileManager::addItem(QString directory, QStandardItem *parent)
{
    QDirIterator it(directory, QDir::Dirs|QDir::NoDotAndDotDot);
    while (it.hasNext())
    {
        QStringList nameFilter("*" + fileExtension);
        QDir dir = it.next();
        QString fileName = dir.dirName().section("/", -1);
        bool validDir = !dir.entryList(nameFilter).empty();

        if (validDir)
        {
            QStandardItem* item = new QStandardItem(fileName);
            item = addItem(directory + "\\" + fileName, item);
            QStandardItem* fullPath = new QStandardItem(directory + "\\" + fileName + "\\" + fileName + fileExtension);

            QList<QStandardItem *> data;
            data.append(item);
            data.append(fullPath);
            parent->appendRow(data);
        }
    }
    return parent;
}

void FileManager::updateSidebar()
{
    QStandardItemModel *model = new QStandardItemModel();
    addItem(homeDirectory, model->invisibleRootItem());
    sidebar->setModel(model);
    sidebar->setColumnHidden(1, true);
}

void FileManager::init(QTreeView *sidebar)
{
    this->sidebar = sidebar;
    updateSidebar();
}

void FileManager::createNewPage()
{
    QString filePath = sidebar->currentIndex().siblingAtColumn(1).data().toString();

    if (filePath == "") filePath = homeDirectory;
    else
    {
        int cutOff = filePath.lastIndexOf(QChar('\\'));
        filePath = filePath.left(cutOff);
    }
    filePath += "\\Untitled Page";

    QDir dir;
    dir.mkpath(filePath);
    QFile file(filePath + "\\Untitled Page.txt");
    file.open(QFile::ReadWrite);
    file.close();

    updateSidebar();
}
