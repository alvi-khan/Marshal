#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

class DatabaseManager
{
public:
    DatabaseManager();

    static void uploadFile(QString filePath, QString tableName);
    static void init();
    static void uploadTo(QString tableName, QString directory);
    static void downloadTo(QString directory, QString table);
    static void saveFile(QString filePath, QByteArray data);
    static bool createNewUser(QString username, QString password);
    static bool authenticateUser(QString username, QString password);
    static QString username;
    static QString privateTable;
    static QString sharedTable;
    static void logoutUser();
    static QString getSharedUsername();
    static void shareFile();
    static bool verifyUsername(QString username);
private:
    static QSqlDatabase db;
    static void loginUser(QString username);
    static QString homeDirectory;
    static QString privateDirectory;
    static QString sharedDirectory;
    static void databaseSetup();
    static QSqlQuery executeQuery(QString queryString);
    static void createTable(QString table);
    static bool userExists(QString username);
    static void addUser(QString username, QString password);
    static bool verifyPassword(QString username, QString password);
};

#endif // DATABASEMANAGER_H
