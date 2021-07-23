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
    QTextBrowser *htmlBlock = qobject_cast<QTextBrowser*>(sender());    // retrieve block that emitted signal
    QString filePath = htmlBlock->documentTitle();  // retrieve file path stored in hidden title
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

/**
 * @brief FileManager::createBlock creates a new block under a parent page
 * @param index is the index of the parent page
 */
void FileManager::createBlock(QModelIndex index)
{
    QString page = index.siblingAtColumn(1).data().toString();  // path to parent file

    // finding a valid block name
    int i = 0;
    QString block = page + "/Block " + QString::number(i) + ".html";
    while (QFileInfo::exists(block) && QFileInfo(block).isFile())
    {
        i++;
        block = page + "/Block " + QString::number(i) + ".html";
    }

    // creating file for block
    QFile file(block);
    if (!file.open(QFile::ReadWrite))
    {
        Error *error = new Error(nullptr, "Error creating file.");
        error->exec();
        return;
    }
    file.close();

    // add block to display
    QTextBrowser* newBlock = Blocks::addHtmlBlock(block);
    if (newBlock != nullptr)   newBlock->setFocus();

    // update parent page's tracker with relative path to new block
    updateFileTracker(page, block.replace(page, "") + "\n");
}

/**
 * @brief FileManager::updateFileTracker adds path to new block to parent page's tracker
 * @param parent is the path to the parent page
 * @param child is the path to add
 */
void FileManager::updateFileTracker(QString parent, QString child)
{
    QFile file(parent + "/files.mar");
    if (!file.open(QFile::Append))
    {
        Error *error = new Error(nullptr, "Error opening file.");
        error->exec();
        return;
    }
    QTextStream content(&file);
    content << child;
    file.close();
}

/**
 * @brief FileManager::getValidFileName retrieves an availble name for a new subpage
 * @param parent is the path to the parent page
 * @return the path to the new subpage
 */
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

/**
 * @brief FileManager::addFile adds a new subpage to a parent page
 * @param index is the index of the parent page
 */
void FileManager::addFile(QModelIndex index)
{
    homeDirectory = "E:/Downloads/Main Folder";
    QString parent;
    // check if parent is specific page or just the root
    if (index.isValid())    parent = index.siblingAtColumn(1).data().toString();
    else                    parent = homeDirectory;

    QDir dir(getValidFileName(parent)); // create tracker for new subpage
    dir.mkpath(dir.path());
    QFile file(dir.path() + "/files.mar");
    if (!file.open(QFile::ReadWrite))
    {
        Error *error = new Error(nullptr, "Error creating file.");
        error->exec();
        return;
    }
    file.close();

    // if parent is a page
    if (parent != homeDirectory)
    {
        updateFileTracker(parent, dir.path().replace(parent, "") + "/files.mar\n"); // update parent's tracker
        Blocks::addSubfileBlock(dir.path() + "/files.mar"); // add subpage block to parent
    }

    // update sidebar
    QStandardItem *parentItem = SidebarManager::getItemAt(index);
    QStandardItem *newItem = SidebarManager::createItem(dir.dirName(), dir.path(), parentItem);

    // for root level files, open them
    if (!index.isValid())
    {
        DisplayManager::openFile(newItem->index());
        SidebarManager::setCurrentIndex(newItem->index());
    }
}

QString FileManager::renameFile(QString oldPath, QString newName)
{
    // create new path to file
    QString newPath = oldPath;
    newPath.truncate(newPath.lastIndexOf(QChar('/')));
    newPath += "/" + newName;

    // rename the file
    QDir dir(oldPath);
    dir.rename(oldPath, newPath);
    return newName;
}
