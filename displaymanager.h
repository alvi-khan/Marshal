#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QFrame>
#include <QLineEdit>
#include <QModelIndex>
#include <QTextBrowser>
#include "addressbar.h"


/**
 * @brief The DisplayManager class is a utility classs that deals with how content is displayed
 */
class DisplayManager : public QObject
{
    Q_OBJECT

public:
    DisplayManager();
    static void init(QFrame *mainPage, QLineEdit *pageTitle, AddressBar *addressBar);
    static QFrame *mainPage;
    static QLineEdit *pageTitle;
    static AddressBar *addressBar;
    static void openFile(QModelIndex index);
    static void renameFile(QModelIndex index);
    static void openFileFromPath(QString filePath, QString title);
    static void createUrl(QString urlFilePath);
public slots:
    void openLink(QUrl url);
};

#endif // DISPLAYMANAGER_H
