#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QModelIndex>
#include <mainwindow.h>


/**
 * @brief The FileManager class is a utility class that handles back-end file management.
 */
class FileManager : public QObject
{
    Q_OBJECT

public:
    FileManager();
    static void addFile(QModelIndex index);
    static QString homeDirectory;
    static QString getValidFileName(QString parent, QString prefix, QString suffix = "");
    static QString renameFile(QString oldPath, QString newName);
    static void createBlock(QModelIndex index);
    static void writeToFile(QString filePath, QString content);
    static void appendToFile(QString filePath, QString content);
    static QString readFromFile(QString filePath);
    static void updateFileTracker(QString parent, QString oldPath, QString newPath);
    static void addCalendar(QModelIndex index);
    static QString openFile;
    static void createURLBlock(QString fileName, QString filePath);
    static void deleteDirectory(QString directory);
public slots:
    void saveBlock();
    static void deletePage(QString pagePath);
};

#endif // FILEMANAGER_H
