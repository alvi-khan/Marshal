#include "filedisplay.h"
#include "filemanager.h"
#include "error.h"

QString FileDisplay::fileExtension;
QTextBrowser * FileDisplay::mainPage;
QLineEdit * FileDisplay::pageTitle;
QTreeView * FileDisplay::sideBar;
QModelIndex FileDisplay::lastIndex;

FileDisplay::FileDisplay()
{

}

void FileDisplay::openFile(QModelIndex index)
{
    if (pageTitle->text() == "" && mainPage->toPlainText() != "")
    {
        Error *error = new Error(nullptr, "Page cannot be saved without a title. \
                                            Content will be lost.\nContinue?");
        error->exec();
        if (!error->errorAccepted())
        {
                sideBar->setCurrentIndex(lastIndex);
                return;
        }
    }

    if (pageTitle->text() == "")    sidebarManager::removeItem();

    QString filePath = index.siblingAtColumn(1).data().toString();
    filePath += "\\" + filePath.section("\\", -1) + fileExtension;

    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        Error *error = new Error(nullptr, "Error opening file.");
        error->exec();
        return;
    }

    QTextStream text(&file);
    mainPage->setText(text.readAll());
    file.close();
    pageTitle->setText(index.data().toString());
    lastIndex = index;
    sideBar->setCurrentIndex(lastIndex);
}

void FileDisplay::saveFile()
{
    QModelIndex index = sideBar->currentIndex();
    QString filePath = index.siblingAtColumn(1).data().toString();
    filePath += "\\" + filePath.section("\\", -1) + fileExtension;

    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        Error *error = new Error(nullptr, "Error opening file.");
        error->exec();
        return;
    }

    QTextStream text(&file);
    text << mainPage->document()->toHtml();
    file.close();
}

void FileDisplay::changeTitle()
{
    QModelIndex index = sideBar->currentIndex();
    QString oldFilePath = index.siblingAtColumn(1).data().toString();
    QString oldFileName = oldFilePath.section("\\", -1);
    // WARNING: Must use file path to derive file name. File name in tree unreliable.

    QString newFileName = pageTitle->text();

    if (oldFileName == "" && newFileName == "") return; // still an empty file
    else if (newFileName == "")
    {
        Error *error = new Error(nullptr, "File name cannot be empty.");
        error->exec();
        pageTitle->setText(oldFileName);
        pageTitle->setFocus();
        sideBar->setCurrentIndex(lastIndex);
        return;
    }
    else if (oldFileName == "" && newFileName != "")
    {
        FileManager::addPage(newFileName);
        return;
    }
    else
    {
        QString newFilePath = oldFilePath;
        newFilePath.truncate(newFilePath.lastIndexOf(QChar('\\')));
        newFilePath += "\\" + newFileName;

        QStandardItemModel *model = (QStandardItemModel *) sideBar->model();
        QStandardItem *item = model->itemFromIndex(index);
        item->setText(newFileName);
        item = model->itemFromIndex(index.siblingAtColumn(1));
        item->setText(newFilePath);

        QDir dir(oldFilePath);
        dir.rename(oldFilePath, newFilePath);

        QFile::rename(newFilePath + "\\" + oldFileName + ".txt", newFilePath + "\\" + newFileName + ".txt");
        return;
    }
}

void FileDisplay::init(QTextBrowser *mainPage, QLineEdit *pageTitle, QTreeView *sideBar)
{
    fileExtension = ".txt";
    FileDisplay::mainPage = mainPage;
    FileDisplay::pageTitle = pageTitle;
    FileDisplay::sideBar = sideBar;
}
