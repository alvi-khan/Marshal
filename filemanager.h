#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QModelIndex>



class FileManager
{
public:
    FileManager();
    static void addFile(QModelIndex index);
    static QString homeDirectory;
    static void updateFileTracker(QString parent, QString child);
    static QString getValidFileName(QString parent);
    static QString renameFile(QString oldPath, QString newName);
};

#endif // FILEMANAGER_H
