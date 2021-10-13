#include "filemanager.h"
#include "sidebarmanager.h"
#include "error.h"
#include "displaymanager.h"
#include "blocks.h"

#include <QDir>

QString FileManager::homeDirectory;
QString FileManager::openFile;

FileManager::FileManager()
{

}

void FileManager::saveBlock()
{
    QTextBrowser *htmlBlock = qobject_cast<QTextBrowser*>(sender());    // retrieve block that emitted signal
    QString filePath = htmlBlock->documentTitle();  // retrieve file path stored in hidden title
    if (filePath != "")
        writeToFile(filePath, htmlBlock->document()->toHtml());
}

/**
 * @brief FileManager::createBlock creates a new block under a parent page
 * @param index is the index of the parent page
 */
void FileManager::createBlock(QModelIndex index)
{
    QString page = index.siblingAtColumn(1).data().toString();  // path to parent file
    QString block = getValidFileName(openFile, "/Block ", ".html");

    // creating file for block
    readFromFile(block);

    // add block to display
    QTextBrowser* newBlock = Blocks::addHtmlBlock(block);
    if (newBlock != nullptr)   newBlock->setFocus();

    // update parent page's tracker with relative path to new block
    appendToFile(openFile + "/files.mar", block.replace(openFile, "") + "\n");
}

void FileManager::createURLBlock(QString fileName, QString filePath)
{
    readFromFile(openFile+"/"+fileName+".url");
    writeToFile(openFile+"/"+fileName+".url", filePath);
    appendToFile(openFile + "/files.mar", "/"+fileName+".url\n");
    DisplayManager::createUrl(openFile+"/"+fileName+".url");
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
    QString parent;
    QString selectedItem = index.siblingAtColumn(1).data().toString();

    if (!index.isValid())   parent = homeDirectory; // root level item
    else                    parent = openFile;

    QDir dir(getValidFileName(parent, "/Untitled Page ")); // create tracker for new subpage
    dir.mkpath(dir.path());
    readFromFile(dir.path() + "/files.mar");

    // if parent is a page
    if (parent != homeDirectory)
    {
        appendToFile(parent + "/files.mar", dir.path().replace(parent, "") + "/files.mar\n");
        Blocks::addSubfileBlock(dir.path() + "/files.mar"); // add subpage block to parent
    }

    // update sidebar if necessary
    if (parent == homeDirectory || parent == selectedItem)
    {
        QStandardItem *parentItem = SidebarManager::getItemAt(index);
        QStandardItem *newItem = SidebarManager::createItem(dir.dirName(), dir.path(), parentItem);

        // for root level files, open them
        if (parent == homeDirectory)
        {
            DisplayManager::openFile(newItem->index());
            SidebarManager::setCurrentIndex(newItem->index());
        }
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

/**
 * @brief FileManager::updateFileTracker replaces the path to an old subpage with the new one
 * @param parent is the path to the parent page
 * @param oldPath is the old subpage path
 * @param newPath is the new subpage path
 */
void FileManager::updateFileTracker(QString parent, QString oldPath, QString newPath)
{
    QString data = readFromFile(parent);
    if (newPath == "")
        data.remove(oldPath + "\n");
    else
        data.replace(oldPath, newPath);
    writeToFile(parent, data);
}

void FileManager::addCalendar(QModelIndex index)
{
    QString parent = index.siblingAtColumn(1).data().toString();
    parent = openFile;

    QDir dir(getValidFileName(parent, "/Calendar ")); // create tracker for calendar
    dir.mkpath(dir.path());
    readFromFile(dir.path() + "/files.cal");

    appendToFile(parent + "/files.mar", dir.path().replace(parent, "") + "/files.cal\n");
    Blocks::addCalendarBlock(dir.path() + "/files.cal");    // add calendar block to parent
}

// TODO might be more appropriate to move this to display manager
void FileManager::deletePage(QString pagePath)
{
    QString parentPath = pagePath;
    parentPath.truncate(pagePath.lastIndexOf(QChar('/')));

    bool parentIsRoot = (parentPath == homeDirectory);

    QModelIndex index = SidebarManager::getCurrentIndex();
    if (pagePath.contains("Calendar"))  // hidden page
    {
        DisplayManager::openFile(index);
        parentPath += "/files.cal";
    }
    else if (!openFile.startsWith(pagePath, Qt::CaseInsensitive))   // other page
    {
        SidebarManager::removeItem(index);
        parentPath += "/files.mar";
    }
    else    // non-hidden page or parent page
    {
        MainWindow *window = (MainWindow *) SidebarManager::sidebar->window();
        window->hideMainPage();
        SidebarManager::removeItem(index);
        parentPath += "/files.mar";
    }

    deleteDirectory(pagePath);

    if(!parentIsRoot)
        updateFileTracker(parentPath, "/" + pagePath.section("/", -1) + "/files.mar", "");

    if (pagePath.startsWith(openFile) && pagePath != openFile)  // subpage deleted; refresh page
        DisplayManager::openFileFromPath(openFile, DisplayManager::pageTitle->text());
}

void FileManager::deleteDirectory(QString directory)
{
    QDir dir(directory);
    dir.removeRecursively();
}


