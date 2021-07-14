#include "filedisplay.h"
#include "filemanager.h"

QString FileDisplay::fileExtension;
QTextBrowser * FileDisplay::mainPage;
QLineEdit * FileDisplay::pageTitle;
QTreeView * FileDisplay::sideBar;

FileDisplay::FileDisplay()
{

}

void FileDisplay::openFile(QModelIndex index)
{
    // TODO if title empty but page not empty, show warning
    if (pageTitle->text() == "")    sidebarManager::removeItem();

    pageTitle->setText(index.data().toString());
    QString filePath = index.siblingAtColumn(1).data().toString();
    filePath += "\\" + filePath.section("\\", -1) + fileExtension;

    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return;
        // TODO show an error message

    QTextStream text(&file);
    mainPage->setText(text.readAll());
    file.close();
}

void FileDisplay::saveFile()
{
    QModelIndex index = sideBar->currentIndex();
    QString filePath = index.siblingAtColumn(1).data().toString();
    filePath += "\\" + filePath.section("\\", -1) + fileExtension;

    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        return;
        // TODO show an error message

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
    else if (newFileName == "")  return;         // TODO show an error and revert to old name
    else if (oldFileName == "" && newFileName != "")
    {
        FileManager::addPage(newFileName);
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
    }
}

void FileDisplay::init(QTextBrowser *mainPage, QLineEdit *pageTitle, QTreeView *sideBar)
{
    fileExtension = ".txt";
    FileDisplay::mainPage = mainPage;
    FileDisplay::pageTitle = pageTitle;
    FileDisplay::sideBar = sideBar;
}
