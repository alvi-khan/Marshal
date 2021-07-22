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
    content << child.replace(parent, "") + "/files.mar\n";
    file.close();
}

QString FileManager::getValidFileName(QString parent)
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
    QString parent = index.siblingAtColumn(1).data().toString();
    if (parent == "")   parent = homeDirectory;

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
        updateFileTracker(parent, dir.path());
        Blocks::addSubfileBlock(dir.path() + "/files.mar");
    }
    SidebarManager::createItem(dir.dirName(), dir.path());
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
