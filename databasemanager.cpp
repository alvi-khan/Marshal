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
bool DatabaseManager::syncing;

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
    if (!db.open()) QMessageBox::warning(MainWindow::window, "Error Connecting", "Unable to connect to database.");
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

/**
 * @brief DatabaseManager::uploadFile uploads a local file to a database table
 * @param filePath is the local path to the file
 * @param tableName is the name of the table to which the file should be uploaded
 * @param shareFilePath is the relative path to the shared file if any
 */
void DatabaseManager::uploadFile(QString filePath, QString tableName, QString shareFilePath)
{
    QString relativePath = filePath;
    relativePath.replace(homeDirectory, "");

    if (tableName.endsWith("_shared"))
    {
        relativePath.replace(shareFilePath, "");    // all shared files should be put at root
        if (relativePath.section("/", 0, 1) == "/Private")
            relativePath = "/Shared/" + relativePath.section("/", 2, -1);
    }

    QString query("INSERT INTO " + tableName + " VALUES ('" + relativePath + "', LOAD_FILE('" + filePath + "'));");
    executeQuery(query);
}

/**
 * @brief DatabaseManager::saveFile saves a file to local storage
 * @param filePath is the local path to which the file will be saved
 * @param data is the file's data
 */
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

/**
 * @brief DatabaseManager::downloadTo downloads the contents of a table to a directory
 */
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

/**
 * @brief DatabaseManager::uploadTo uploads the contents of a table to a directory
 * @param shareFilePath is the shared file path if any
 */
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
    syncing = false;

    if (username != "")
        loginUser(username);
}

void DatabaseManager::loginUser(QString username)
{
    DatabaseManager::username = username;
    privateTable = username + "_private";
    sharedTable = username + "_shared";

    FileManager::writeToFile(QDir::currentPath() + "/user.dat", username);

    // synchronizing in background
    MainWindow::window->toggleLoadingGIF();
    syncing = true;
    QFuture<void> task = QtConcurrent::run(syncFiles);
    QFutureWatcher<void> *taskWatcher = new QFutureWatcher<void>();
    taskWatcher->setFuture(task);
    connect(taskWatcher, &QFutureWatcher<void>::finished, [=] {
        SidebarManager::reloadSidebar();
        MainWindow::window->toggleLoadingGIF();
        syncing = false;
    });
}

void DatabaseManager::logoutUser()
{
    username = "";
    privateTable = "";
    FileManager::writeToFile(QDir::currentPath() + "/user.dat", "");
    FileManager::deleteDirectory(sharedDirectory);
    SidebarManager::reloadSidebar();
}

/**
 * @brief DatabaseManager::getSharedUsername reveals dialog to pich user to share with
 */
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
        QMessageBox::warning(MainWindow::window, "Login", "Please log in before sharing files.");
        return;
    }

    QString sharedUser = getSharedUsername();

    if (sharedUser == "")   return;
    else if (!verifyUsername(sharedUser))
    {
        QMessageBox::warning(MainWindow::window, "User Not Found", "The user could not be found.");
        return;
    }
    else if (sharedUser == username)
    {
        QMessageBox::warning(MainWindow::window, "Invalid User", "You cannot share files with yourself.");
        return;
    }

    QString sharedTable = sharedUser + "_shared";
    QString sharedFile = FileManager::openFile;
    QString sharedFilePath = sharedFile.section("/", 0, -2);

    // share file in background
    MainWindow::window->toggleLoadingGIF();
    syncing = true;
    QFuture<void> task1 = QtConcurrent::run(createTable, sharedTable);
    QFutureWatcher<void> *task1Watcher = new QFutureWatcher<void>();
    task1Watcher->setFuture(task1);
    connect(task1Watcher, &QFutureWatcher<void>::finished, [=] {
        QFuture<void> task2 = QtConcurrent::run(uploadTo, sharedTable, sharedFile, sharedFilePath);
        QFutureWatcher<void> *task2Watcher = new QFutureWatcher<void>();
        task2Watcher->setFuture(task2);
        connect(task2Watcher, &QFutureWatcher<void>::finished, [=] {
            MainWindow::window->toggleLoadingGIF();
            syncing = false;
        });
    });
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
