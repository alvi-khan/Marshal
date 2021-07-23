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
    writeToFile(filePath, htmlBlock->document()->toHtml());
}

/**
 * @brief FileManager::createBlock creates a new block under a parent page
 * @param index is the index of the parent page
 */
void FileManager::createBlock(QModelIndex index)
{
    QString page = index.siblingAtColumn(1).data().toString();  // path to parent file
    QString block = getValidFileName(page, "/Block ", ".html");

    // creating file for block
    readFromFile(block);

    // add block to display
    QTextBrowser* newBlock = Blocks::addHtmlBlock(block);
    if (newBlock != nullptr)   newBlock->setFocus();

    // update parent page's tracker with relative path to new block
    updateFileTracker(page, block.replace(page, "") + "\n");
}

void FileManager::writeToFile(QString filePath, QString content)
{
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly))
    {
        Error *error = new Error(nullptr, "Error opening file.");
        error->exec();
        return;
    }
    QTextStream text(&file);
    text << content;
    file.close();
}

void FileManager::appendToFile(QString filePath, QString content)
{
    QFile file(filePath);
    if (!file.open(QFile::Append))
    {
        Error *error = new Error(nullptr, "Error opening file.");
        error->exec();
        return;
    }
    QTextStream text(&file);
    text << content;
    file.close();
}

QString FileManager::readFromFile(QString filePath)
{
    QString data = "";
    QFile file(filePath);
    if (!file.open(QFile::ReadWrite))
    {
        Error *error = new Error(nullptr, "Error opening file.");
        error->exec();
        return data;
    }
    QTextStream content(&file);
    data = content.readAll();
    file.close();
    return data;
}

/**
 * @brief FileManager::updateFileTracker adds path to new block to parent page's tracker
 * @param parent is the path to the parent page
 * @param child is the path to add
 */
void FileManager::updateFileTracker(QString parent, QString child)
{
    appendToFile(parent + "/files.mar", child);
}

/**
 * @brief FileManager::getValidFileName generates a valid name for a subfile/block
 * @param parent is the path to the parent folder
 * @param prefix is the prefix required for the new file
 * @param suffix is the optional suffix for the new file
 * @return the path to the new file
 */
QString FileManager::getValidFileName(QString parent, QString prefix, QString suffix)
{
    int i = 0;
    QString path = parent + prefix + QString::number(i) + suffix;
    while (QDir().exists(path))
    {
        i++;
        path = parent + prefix + QString::number(i) + suffix;
    }
    return path;
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

    QDir dir(getValidFileName(parent, "/Untitled Page ")); // create tracker for new subpage
    dir.mkpath(dir.path());
    readFromFile(dir.path() + "/files.mar");

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
