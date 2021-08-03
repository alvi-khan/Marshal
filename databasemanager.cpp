#include "databasemanager.h"
#include "sidebarmanager.h"
#include "filemanager.h"

QSqlDatabase DatabaseManager::db;
QString DatabaseManager::username;
QString DatabaseManager::privateTable;
QString DatabaseManager::homeDirectory;

DatabaseManager::DatabaseManager()
{

}

void DatabaseManager::databaseSetup()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("marshal_user_files");
    db.setPort(3306);
}

QSqlQuery DatabaseManager::executeQuery(QString queryString)
{
    if (!db.open()) QMessageBox::warning(nullptr, "Error Connecting", "Unable to connect to database.");
    QSqlQuery query(queryString);
    query.exec();
    db.close();
    return query;
}

void DatabaseManager::createPrivateTable()
{
    QString query( "CREATE TABLE IF NOT EXISTS " + privateTable + "(filepath VARCHAR(260), file LONGBLOB, PRIMARY KEY (filepath))");
    executeQuery(query);
}

void DatabaseManager::uploadFile(QString filePath)
{
    QString relativePath = filePath;
    relativePath.replace(homeDirectory, "");

    QString query("INSERT INTO " + privateTable + " VALUES ('" + relativePath + "', LOAD_FILE('" + filePath + "'));");
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

void DatabaseManager::restore()
{
    QString queryString("SELECT * FROM " + privateTable + ";");
    QSqlQuery query = executeQuery(queryString);

    while(query.next())
    {
        QString filePath = homeDirectory + query.value(0).toString();
        QByteArray data = query.value(1).toByteArray();
        if (!QDir().exists(filePath))
            saveFile(filePath, data);
    }
}

void DatabaseManager::backup()
{
    QDirIterator it(homeDirectory, QDir::Files | QDir::NoDotAndDotDot,  QDirIterator::Subdirectories);
    while (it.hasNext())
        uploadFile(it.next());
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
    databaseSetup();

    homeDirectory = "E:/Downloads/Main Folder";
    username = FileManager::readFromFile(QDir::currentPath() + "/user.dat");
    privateTable = "";

    if (username != "")
    {
        privateTable = username + "_private";
        createPrivateTable();
        backup();
        restore();
    }
}

void DatabaseManager::loginUser(QString username)
{
    DatabaseManager::username = username;
    privateTable = username + "_private";

    FileManager::writeToFile(QDir::currentPath() + "/user.dat", username);

    createPrivateTable();
    backup();
    restore();

    SidebarManager::reloadSidebar();
}

void DatabaseManager::logoutUser()
{
    username = "";
    privateTable = "";
    FileManager::writeToFile(QDir::currentPath() + "/user.dat", "");
}
