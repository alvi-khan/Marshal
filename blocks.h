#ifndef BLOCKS_H
#define BLOCKS_H

#include <QFrame>
#include <QObject>
#include <QTextBrowser>



class Blocks : public QObject
{
public:
    Blocks();
    static QTextBrowser* createTextBrowser(QString content);
    static QFrame *mainPage;
    static void addHtmlBlock(QString filePath);
    static void addLinkBlock(QString link, QString name);
    static void addSubfileBlock(QString filePath);
    static void init(QFrame *mainPage);
};

#endif // BLOCKS_H
