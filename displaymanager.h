#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QFrame>
#include <QLineEdit>
#include <QModelIndex>
#include <QTextBrowser>



class DisplayManager : public QObject
{
    Q_OBJECT

public:
    DisplayManager();
    static void init(QFrame *mainPage, QLineEdit *pageTitle);
    static QFrame *mainPage;
    static QLineEdit *pageTitle;
    static void openFile(QModelIndex index);
    static void addHtmlBlock(QString filePath);
    static void renameFile(QModelIndex index);
    static void addSubfileBlock(QString filePath);
    static void addUrlBlock(QString url, QString displayName);
    static void createUrl(QString url, QString displayName);
public slots:
    void saveBlock();
    void openLink(QUrl url);
};

#endif // DISPLAYMANAGER_H
