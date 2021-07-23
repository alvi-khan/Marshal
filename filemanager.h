#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QModelIndex>


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
    static void updateFileTracker(QString parent, QString child);
    static QString getValidFileName(QString parent);
    static QString renameFile(QString oldPath, QString newName);
    static void createBlock(QModelIndex index);
    static void writeToFile(QString filePath, QString content);
    static void appendToFile(QString filePath, QString content);
    static QString readFromFile(QString filePath);
public slots:
    void saveBlock();
};

#endif // FILEMANAGER_H
