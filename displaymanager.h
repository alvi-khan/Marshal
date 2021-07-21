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
public slots:
    void saveBlock();
};

#endif // DISPLAYMANAGER_H
