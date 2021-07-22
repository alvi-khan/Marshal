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
    if (!link.endsWith(".mar")) QDesktopServices::openUrl(url);
    else
    {
        link.truncate(link.lastIndexOf(QChar('/')));
        QModelIndex index = SidebarManager::getChild(link);
        SidebarManager::setCurrentIndex(index);
        openFile(index);
    }
}

void DisplayManager::createUrl(QString url, QString displayName)
{
    // new dialog, get url
    Blocks::addLinkBlock(url, displayName);
    // save block as file
    // update file tracker
}

/**
 * @brief DisplayManager::renameFile Verifies new name is valid and calls functions in file manager and sidebar to change name
 * @param index
 */
void DisplayManager::renameFile(QModelIndex index)
{
    QString oldPath = index.siblingAtColumn(1).data().toString();
    QString oldName = oldPath.section("/", -1);
    // WARNING: Must use file path to derive file name. File name in tree unreliable.
    if (pageTitle->text() == "")
    {
        Error *error = new Error(nullptr, "File name cannot be empty.");
        error->exec();
        pageTitle->setText(oldName);
        return;
    }

    QString newName = FileManager::renameFile(oldPath, pageTitle->text());
    SidebarManager::rename(index, newName);
    openFile(index);
}

/**
 * @brief DisplayManager::openFile parses all the blocks in a file
 * @param index
 */
void DisplayManager::openFile(QModelIndex index)
{
    while (QWidget *w = mainPage->findChild<QWidget*>())    delete w;

    QString filePath = index.siblingAtColumn(1).data().toString();
    QFile file(filePath + "/files.mar");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Error *error = new Error(nullptr, "Error opening file.");
        error->exec();
        return;
    }

    QTextStream blocks(&file);



    while (!blocks.atEnd())
    {
        QFileInfo block(filePath + blocks.readLine());
        if (block.completeSuffix() == "html")
            Blocks::addHtmlBlock(block.absoluteFilePath());
        else if (block.completeSuffix() == "mar")
            Blocks::addSubfileBlock(block.absoluteFilePath());
    }

    file.close();

    pageTitle->setText(index.data().toString());
}

void DisplayManager::init(QFrame *mainPage, QLineEdit *pageTitle)
{
    DisplayManager::mainPage = mainPage;
    DisplayManager::pageTitle = pageTitle;
}
