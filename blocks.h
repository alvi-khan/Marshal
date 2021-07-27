#ifndef BLOCKS_H
#define BLOCKS_H

#include <QFrame>
#include <QObject>
#include <QTextBrowser>


/**
 * @brief The Blocks class is a utility class that deals with blocks inside the currently opened page
 */
class Blocks : public QObject
{
    Q_OBJECT
public:
    Blocks();
    static QTextBrowser* createTextBrowser(QString content);
    static QFrame *mainPage;
    static QTextBrowser* addHtmlBlock(QString filePath);
    static void addLinkBlock(QString link, QString name);
    static void addSubfileBlock(QString filePath);
    static void init(QFrame *mainPage);
    static void addCalendarBlock(QString filePath);
public slots:
    void updateBlockSize();
};

#endif // BLOCKS_H
