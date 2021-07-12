#ifndef FILEDISPLAY_H
#define FILEDISPLAY_H

#include<QStandardItemModel>
#include<QTextBrowser>
#include<QFile>
#include<QLineEdit>
#include<QDir>

class FileDisplay
{
public:
    FileDisplay();
    void openFile(QModelIndex index, QTextBrowser *content, QLineEdit *title);
    void saveFile(QModelIndex index, QTextBrowser *content);
    void changeTitle(QStandardItemModel *model, QModelIndex index, QLineEdit *title);
private:
    QString fileExtension;
};

#endif // FILEDISPLAY_H
