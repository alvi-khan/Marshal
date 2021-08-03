#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

class DatabaseManager
{
public:
    DatabaseManager();

    static void uploadFile(QString filePath);
    static void init();
    static void backup();
    static void restore();
    static void saveFile(QString filePath, QByteArray data);
    static bool createNewUser(QString username, QString password);
    static bool authenticateUser(QString username, QString password);
    static QString username;
    static QString privateTable;
    static void logoutUser();
private:
    static QSqlDatabase db;
    static void loginUser(QString username);
    static QString homeDirectory;
    static void databaseSetup();
    static QSqlQuery executeQuery(QString queryString);
    static void createPrivateTable();
    static bool userExists(QString username);
    static void addUser(QString username, QString password);
    static bool verifyPassword(QString username, QString password);
};

#endif // DATABASEMANAGER_H
