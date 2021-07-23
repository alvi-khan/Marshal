#include "filemanager.h"
#include "sidebarmanager.h"
#include "error.h"
#include "displaymanager.h"
#include "blocks.h"

#include <QDir>

QString FileManager::homeDirectory;

FileManager::FileManager()
{

}

void FileManager::saveBlock()
{
    QTextBrowser *htmlBlock = qobject_cast<QTextBrowser*>(sender());
    QString filePath = htmlBlock->documentTitle();
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly))
    {
        Error *error = new Error(nullptr, "Error saving file.");
        error->exec();
        return;
    }
    QTextStream text(&file);
    text << htmlBlock->document()->toHtml();
    file.close();
}

void FileManager::createBlock(QModelIndex index)
{
    QString page = index.siblingAtColumn(1).data().toString();

    int i = 0;
    QString block = page + "/Block " + QString::number(i) + ".html";
    while (QFileInfo::exists(block) && QFileInfo(block).isFile())
    {
        i++;
        block = page + "/Block " + QString::number(i) + ".html";
    }
    QFile file(block);
    if (!file.open(QFile::ReadWrite))
    {
        Error *error = new Error(nullptr, "Error creating file.");
        error->exec();
        return;
    }
    file.close();
    QTextBrowser* newBlock = Blocks::addHtmlBlock(block);
    if (newBlock != nullptr)   newBlock->setFocus();
    updateFileTracker(page, block.replace(page, "") + "\n");
}

void FileManager::updateFileTracker(QString parent, QString child)
{
    QFile file(parent + "/files.mar");
    if (!file.open(QFile::Append))
    {
        Error *error = new Error(nullptr, "Error creating file.");
        error->exec();
        return;
    }
    QTextStream content(&file);
    content << child;
    file.close();
}

QString FileManager::getValidFileName(QString parent)  // gets valid name for untitled page
{
    int i = 0;
    QDir dir(parent + "/Untitled Page " + QString::number(i));
    while (dir.exists())
    {
        i++;
        dir.setPath(parent + "/Untitled Page " + QString::number(i));
    }
    return dir.path();
}

void FileManager::addFile(QModelIndex index)
{
    homeDirectory = "E:/Downloads/Main Folder";
    QString parent;
    if (index.isValid())    parent = index.siblingAtColumn(1).data().toString();
    else                    parent = homeDirectory;

    QDir dir(getValidFileName(parent));
    dir.mkpath(dir.path());
    QFile file(dir.path() + "/files.mar");
    if (!file.open(QFile::ReadWrite))
    {
        Error *error = new Error(nullptr, "Error creating file.");
        error->exec();
        return;
    }
    file.close();

    if (parent != homeDirectory)
    {
        updateFileTracker(parent, dir.path().replace(parent, "") + "/files.mar\n");
        Blocks::addSubfileBlock(dir.path() + "/files.mar");
    }
    QStandardItem *newItem = SidebarManager::createItem(dir.dirName(), dir.path(), SidebarManager::getItemAt(index));

    if (!index.isValid())
    {
        DisplayManager::openFile(newItem->index());
        SidebarManager::setCurrentIndex(newItem->index());
    }
}

QString FileManager::renameFile(QString oldPath, QString newName)
{
    QString newPath = oldPath;
    newPath.truncate(newPath.lastIndexOf(QChar('/')));
    newPath += "/" + newName;
    QDir dir(oldPath);
    dir.rename(oldPath, newPath);
    return newName;
}
