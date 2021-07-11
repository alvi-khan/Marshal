#include "filedisplay.h"

FileDisplay::FileDisplay()
{

}

void FileDisplay::openFile(QModelIndex index, QTextBrowser *content, QTextEdit *title)
{
    title->setText(index.data().toString());
    QString filePath = index.siblingAtColumn(1).data().toString();

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

    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        return;
        // TODO show an error message

    QTextStream text(&file);
    text << content->document()->toHtml();
    file.close();
}

void FileDisplay::changeTitle(QModelIndex index, QTextEdit *title)
{
    QString filePath = index.siblingAtColumn(1).data().toString();
    QString fileName = title->document()->toHtml();
    QFile file(filePath);
    file.rename(fileName);
}
