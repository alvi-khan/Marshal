#include "databasemanager.h"
#include "sidebarmanager.h"
#include "filemanager.h"
#include "filesharedialog.h"

#include <QtConcurrent/QtConcurrent>
#include <qtconcurrentrun.h>
#include <QThread>
#include <QtConcurrentRun>

QSqlDatabase DatabaseManager::db;
QString DatabaseManager::username;
QString DatabaseManager::privateTable;
QString DatabaseManager::sharedTable;
QString DatabaseManager::homeDirectory;
QString DatabaseManager::privateDirectory;
QString DatabaseManager::sharedDirectory;

DatabaseManager::DatabaseManager()
{

}

QSqlDatabase DatabaseManager::databaseSetup()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("marshal_user_files");
    db.setPort(3306);
    return db;
}

QSqlQuery DatabaseManager::executeQuery(QString queryString)
{
    QSqlDatabase db = databaseSetup();
    if (!db.open()) QMessageBox::warning(nullptr, "Error Connecting", "Unable to connect to database.");
    QSqlQuery query(queryString);
    query.exec();
    db.close();
    return query;
}

void DatabaseManager::createTable(QString table)
{
    QString query( "CREATE TABLE IF NOT EXISTS " + table + "(filepath VARCHAR(260), file LONGBLOB, PRIMARY KEY (filepath))");
    executeQuery(query);
}

void DatabaseManager::uploadFile(QString filePath, QString tableName, QString shareFilePath)
{
    QString relativePath = filePath;
    relativePath.replace(homeDirectory, "");

    if (tableName.endsWith("_shared"))
    {
        relativePath = filePath;
        relativePath.replace(shareFilePath, "");
        relativePath = "/Shared/" + relativePath;
    }

    QString query("INSERT INTO " + tableName + " VALUES ('" + relativePath + "', LOAD_FILE('" + filePath + "'));");
    executeQuery(query);
}

void DatabaseManager::saveFile(QString filePath, QByteArray data)
{
    QString folder = filePath.section("/", 0, -2);
    QDir dir;
    if (!dir.exists(folder))    dir.mkpath(folder);

    QFile file(filePath);
    file.open(QFile::WriteOnly);
    file.write(data);
    file.close();
}

void DatabaseManager::downloadTo(QString directory, QString table)
{
    QString queryString("SELECT * FROM " + table + ";");
    QSqlQuery query = executeQuery(queryString);

    while(query.next())
    {
        QString filePath = directory + query.value(0).toString();
        QByteArray data = query.value(1).toByteArray();
        if (!QDir().exists(filePath))
            saveFile(filePath, data);
    }
}

void DatabaseManager::uploadTo(QString table, QString directory, QString shareFilePath)
{
    QDirIterator it(directory, QDir::Files | QDir::NoDotAndDotDot,  QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString directory = it.next();
        uploadFile(directory, table, shareFilePath);
    }
}

void DatabaseManager::addUser(QString username, QString password)
{
    QString query("INSERT INTO user_list VALUES ('" + username + "', '" + password + "');");
    executeQuery(query);
}

bool DatabaseManager::userExists(QString username)
{
    QString queryString("SELECT username FROM user_list WHERE username = '" + username +"';");
    QSqlQuery query = executeQuery(queryString);
    while (query.next())    return true;
    return false;
}

bool DatabaseManager::createNewUser(QString username, QString password)
{
    if (!userExists(username))
    {
        // TODO switch to stronger encryption
        password = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex());
        addUser(username, password);
        loginUser(username);
        return true;
    }
    return false;
}

bool DatabaseManager::verifyPassword(QString username, QString password)
{
    QString queryString("SELECT * FROM user_list WHERE username = '" + username + "' AND password = '" + password + "';");
    QSqlQuery query = executeQuery(queryString);
    while (query.next())    return true;
    return false;
}

bool DatabaseManager::authenticateUser(QString username, QString password)
{
    if (userExists(username))
    {
        // TODO switch to stronger encryption
        password = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex());
        if (verifyPassword(username, password))
        {
            loginUser(username);
            return true;
        }
        return false;
    }
    return false;
}

void DatabaseManager::init()
{
    homeDirectory = FileManager::homeDirectory.section("/", 0, -2);
    privateDirectory = homeDirectory + "/Private";
    sharedDirectory = homeDirectory + "/Shared";
    username = FileManager::readFromFile(QDir::currentPath() + "/user.dat");
    privateTable = "";
    sharedTable = "";

    if (username != "")
        loginUser(username);
}

void DatabaseManager::loginUser(QString username)
{
    DatabaseManager::username = username;
    privateTable = username + "_private";
    sharedTable = username + "_shared";

    FileManager::writeToFile(QDir::currentPath() + "/user.dat", username);

    QFuture<void> task = QtConcurrent::run(syncFiles);
    QFutureWatcher<void> *taskWatcher = new QFutureWatcher<void>();
    taskWatcher->setFuture(task);
    connect(taskWatcher, &QFutureWatcher<void>::finished, [=] {SidebarManager::reloadSidebar();});
}

void DatabaseManager::logoutUser()
{
    username = "";
    privateTable = "";
    FileManager::writeToFile(QDir::currentPath() + "/user.dat", "");
    FileManager::deleteDirectory(sharedDirectory);
    SidebarManager::reloadSidebar();
}

QString DatabaseManager::getSharedUsername()
{
    FileShareDialog *fileShareDialog = new FileShareDialog();
    fileShareDialog->exec();

    QString username = fileShareDialog->username;
    delete fileShareDialog;
    return username;
}

bool DatabaseManager::verifyUsername(QString username)
{
    QString queryString("SELECT * FROM user_list WHERE username = '" + username + "';");
    QSqlQuery query = executeQuery(queryString);
    while (query.next())    return true;
    return false;
}

void DatabaseManager::shareFile()
{
    if (username == "")
    {
        QMessageBox::warning(nullptr, "Login", "Please log in before sharing files.");
        return;
    }

    QString sharedUser = getSharedUsername();

    if (sharedUser == "")   return;
    else if (!verifyUsername(sharedUser))
    {
        QMessageBox::warning(nullptr, "User Not Found", "The user could not be found.");
        return;
    }
    else if (sharedUser == username)
    {
        QMessageBox::warning(nullptr, "Invalid User", "You cannot share files with yourself.");
        return;
    }

    QString sharedTable = sharedUser + "_shared";
    QString sharedFile = FileManager::openFile;
    QString sharedFilePath = sharedFile.section("/", 0, -2);

    createTable(sharedTable);
    uploadTo(sharedTable, sharedFile, sharedFilePath);
}

QList<QString> DatabaseManager::getUserList()
{
    QString queryString = "SELECT username FROM user_list;";
    QSqlQuery query = executeQuery(queryString);
    QList<QString> userList;
    while(query.next())
    {
        QString user = query.value(0).toString();
        if (user != username)   userList.append(user);
    }
    return userList;
}

void DatabaseManager::syncFiles()
{
    createTable(privateTable);
    createTable(sharedTable);
    uploadTo(privateTable, privateDirectory);
    uploadTo(sharedTable, sharedDirectory);
    downloadTo(homeDirectory, privateTable);
    downloadTo(homeDirectory, sharedTable);
}
