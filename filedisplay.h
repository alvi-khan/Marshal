#ifndef FILEDISPLAY_H
#define FILEDISPLAY_H

#include<QStandardItemModel>
#include<QTextBrowser>
#include<QFile>

class FileDisplay
{
public:
    FileDisplay();
    void openFile(QModelIndex index, QTextBrowser *content, QTextEdit *title);
    void saveFile(QModelIndex index, QTextBrowser *content);
    void changeTitle(QModelIndex index, QTextEdit *title);
};

#endif // FILEDISPLAY_H
