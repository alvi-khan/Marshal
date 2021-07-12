#include "filedisplay.h"

FileDisplay::FileDisplay()
{
    fileExtension = ".txt";
}

void FileDisplay::openFile(QModelIndex index, QTextBrowser *content, QLineEdit *title)
{
    title->setText(index.data().toString());
    QString filePath = index.siblingAtColumn(1).data().toString();
    filePath += "\\" + filePath.section("\\", -1) + fileExtension;

    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return;
        // TODO show an error message

    QTextStream text(&file);
    content->setText(text.readAll());
    file.close();
}

void FileDisplay::saveFile(QModelIndex index, QTextBrowser *content)
{
    QString filePath = index.siblingAtColumn(1).data().toString();
    filePath += "\\" + filePath.section("\\", -1) + fileExtension;

    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        return;
        // TODO show an error message

    QTextStream text(&file);
    text << content->document()->toHtml();
    file.close();
}

void FileDisplay::changeTitle(QStandardItemModel *model, QModelIndex index, QLineEdit *title)
{
    QString oldFilePath = index.siblingAtColumn(1).data().toString();
    QString oldFileName = oldFilePath.section("\\", -1);

    QString newFileName = title->text();
    if (newFileName == "")  return;

    QString newFilePath = oldFilePath;
    newFilePath.truncate(newFilePath.lastIndexOf(QChar('\\')));
    newFilePath += "\\" + newFileName;

    QStandardItem *item = model->itemFromIndex(index);
    item->setText(newFileName);
    item = model->itemFromIndex(index.siblingAtColumn(1));
    item->setText(newFilePath);

    QDir dir(oldFilePath);
    dir.rename(oldFilePath, newFilePath);

    QFile::rename(newFilePath + "\\" + oldFileName + ".txt", newFilePath + "\\" + newFileName + ".txt");
}
