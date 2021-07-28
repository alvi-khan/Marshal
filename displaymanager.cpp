#include "blocks.h"
#include "displaymanager.h"

#include <QCalendarWidget>
#include <QDir>
#include <QFile>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QLabel>
#include "error.h"
#include "filemanager.h"
#include "sidebarmanager.h"

QFrame * DisplayManager::mainPage;
QLineEdit * DisplayManager::pageTitle;

DisplayManager::DisplayManager()
{

}

void DisplayManager::openLink(QUrl url)
{
    QString link = url.toString();

    // normal browser link; open in default browser
    if (!link.endsWith(".mar")) QDesktopServices::openUrl(QUrl::fromLocalFile(link));
    else
    {
        // subfile link
        link.truncate(link.lastIndexOf(QChar('/')));
        QString fileName = link.section("/", -1);
        QModelIndex index = SidebarManager::getChild(link); // get index of subfile
        SidebarManager::setCurrentIndex(index);
        //openFile(index);
        openFileFromPath(link, fileName);
    }
}

void DisplayManager::createUrl(QString url, QString displayName)
{
    // new dialog, get url
    Blocks::addLinkBlock(url, displayName);
    // save block as file
    // update file tracker
}

void DisplayManager::renameFile(QModelIndex index)
{
    // Must use file path to derive file name. File name in tree unreliable.
    QString oldPath = index.siblingAtColumn(1).data().toString();
    oldPath = FileManager::openFile;
    QString oldName = oldPath.section("/", -1);

    if (pageTitle->text() == "")
    {
        Error *error = new Error(nullptr, "File name cannot be empty.");
        error->exec();
        pageTitle->setText(oldName);
        return;
    }

    QString newName = FileManager::renameFile(oldPath, pageTitle->text());

    if (index.siblingAtColumn(1).data().toString() == FileManager::openFile)
    {
        SidebarManager::rename(index, newName);
    }
    else
    {
        QString parentPath = FileManager::openFile.section("/", 0, -2);
        qDebug()<<parentPath;

        FileManager::updateFileTracker(parentPath + "/files.cal", "/" + oldName + "/files.mar", "/" + newName + "/files.mar");
    }


    if (index.parent().isValid())
    {
        QString parentPath = index.parent().siblingAtColumn(1).data().toString();
        FileManager::updateFileTracker(parentPath + "/files.mar", "/" + oldName + "/files.mar", "/" + newName + "/files.mar");
    }

    FileManager::openFile.truncate(FileManager::openFile.lastIndexOf(QChar('/')));
    FileManager::openFile += "/" + newName;

    //openFile(index);
    openFileFromPath(FileManager::openFile, newName);
}

void DisplayManager::openFileFromPath(QString filePath, QString title)
{
    FileManager::openFile = filePath;

    // removing existing blocks
    QList<QObject *> objects = mainPage->children();
    foreach (QObject *object, objects)
    {
        QWidget *widget = (QWidget *) object;
        widget->hide();
    }

    QFile file(filePath + "/files.mar");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Error *error = new Error(nullptr, "Error opening file.");
        error->exec();
        return;
    }

    // retrieve relative paths to all blocks
    QTextStream blocks(&file);

    // for each block, open appropriately
    while (!blocks.atEnd())
    {
        QFileInfo block(filePath + blocks.readLine());      // absolute path from relative path
        if (block.completeSuffix() == "html")
            Blocks::addHtmlBlock(block.absoluteFilePath());
        else if (block.completeSuffix() == "mar")
            Blocks::addSubfileBlock(block.absoluteFilePath());
        else if (block.completeSuffix() == "cal")
            Blocks::addCalendarBlock(block.absoluteFilePath());
    }

    file.close();

    pageTitle->setText(title);
}

void DisplayManager::openFile(QModelIndex index)
{
    QString filePath = index.siblingAtColumn(1).data().toString();
    openFileFromPath(filePath, index.data().toString());
}

void DisplayManager::init(QFrame *mainPage, QLineEdit *pageTitle)
{
    DisplayManager::mainPage = mainPage;
    DisplayManager::pageTitle = pageTitle;
}
