#include "displaymanager.h"

#include <QCalendarWidget>
#include <QDir>
#include <QFile>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QLabel>
#include "error.h"
#include "filemanager.h"
#include "sidebarmanager.h"

QFrame * DisplayManager::mainPage;
QLineEdit * DisplayManager::pageTitle;

DisplayManager::DisplayManager()
{

}

void DisplayManager::saveBlock()
{
    QTextBrowser *htmlBlock = qobject_cast<QTextBrowser*>(sender());
    QString filePath = htmlBlock->documentTitle();
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly))
    {
        Error *error = new Error(nullptr, "Error saving file.");
        error->exec();
        return;
    }
    QTextStream text(&file);
    text << htmlBlock->document()->toHtml();
    file.close();
}

/**
 * @brief DisplayManager::addHtmlBlock adds a block of HTML content to the main page
 * @param filePath
 */
void DisplayManager::addHtmlBlock(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Error *error = new Error(nullptr, "Error opening file.");
        error->exec();
        return;
    }
    QTextStream content(&file);
    QTextBrowser *htmlBlock = new QTextBrowser();
    htmlBlock->setHtml(content.readAll());
    htmlBlock->setDocumentTitle(filePath);
    htmlBlock->setReadOnly(false);
    htmlBlock->setFrameStyle(QFrame::NoFrame);
    htmlBlock->setTextColor(QColor::fromRgb(255, 255, 255));

    QFontMetrics m(htmlBlock->font());
    int rowHeight = m.lineSpacing();
    htmlBlock->setMaximumHeight(rowHeight * htmlBlock->document()->lineCount() * 1.5);

    connect(htmlBlock, &QTextEdit::textChanged, new DisplayManager(), &DisplayManager::saveBlock);

    QVBoxLayout *layout = (QVBoxLayout *) mainPage->layout();
    layout->addWidget(htmlBlock);
    mainPage->setLayout(layout);
    file.close();
}

void DisplayManager::openLink(QUrl url)
{
    QString link = url.toString();
    if (!link.endsWith(".mar")) QDesktopServices::openUrl(url);
    else
    {
        link.truncate(link.lastIndexOf(QChar('/')));
        QModelIndex index = SidebarManager::getChild(link);
        SidebarManager::setCurrentIndex(index);
        openFile(index);
    }
}

void DisplayManager::addSubfileBlock(QString filePath)
{
    QString fileName = filePath.section("/", -2, -2);

    QTextBrowser *subfileBlock = new QTextBrowser();
    subfileBlock->insertHtml("<a style=\"color: #66d9ee\" href=\"" + filePath + "\">" + fileName + "</a>");
    subfileBlock->setOpenLinks(false);
    subfileBlock->setFrameStyle(QFrame::NoFrame);
    subfileBlock->setTextColor(QColor::fromRgb(102, 217, 238));

    QFontMetrics m(subfileBlock->font());
    int rowHeight = m.lineSpacing();
    subfileBlock->setMaximumHeight(rowHeight * subfileBlock->document()->lineCount() * 1.5);

    subfileBlock->connect(subfileBlock, &QTextBrowser::anchorClicked, new DisplayManager(), &DisplayManager::openLink);

    QVBoxLayout *layout = (QVBoxLayout *) mainPage->layout();
    layout->addWidget(subfileBlock);
    mainPage->setLayout(layout);
}

void DisplayManager::addUrlBlock(QString url, QString displayName)
{
    QTextBrowser *urlBlock = new QTextBrowser();
    urlBlock->insertHtml("<a style=\"color: #66d9ee\" href=\"" + url + "\">" + displayName + "</a>");
    urlBlock->setOpenLinks(false);
    urlBlock->setFrameStyle(QFrame::NoFrame);
    urlBlock->setTextColor(QColor::fromRgb(102, 217, 238));

    QFontMetrics m(urlBlock->font());
    int rowHeight = m.lineSpacing();
    urlBlock->setMaximumHeight(rowHeight * urlBlock->document()->lineCount() * 1.5);

    urlBlock->connect(urlBlock, &QTextBrowser::anchorClicked, new DisplayManager(), &DisplayManager::openLink);

    QVBoxLayout *layout = (QVBoxLayout *) mainPage->layout();
    layout->addWidget(urlBlock);
    mainPage->setLayout(layout);
}

void DisplayManager::createUrl(QString url, QString displayName)
{
    // new dialog, get url
    addUrlBlock(url, displayName);
    // save block as file
    // update file tracker
}

/**
 * @brief DisplayManager::renameFile Verifies new name is valid and calls functions in file manager and sidebar to change name
 * @param index
 */
void DisplayManager::renameFile(QModelIndex index)
{
    QString oldPath = index.siblingAtColumn(1).data().toString();
    QString oldName = oldPath.section("/", -1);
    // WARNING: Must use file path to derive file name. File name in tree unreliable.
    if (pageTitle->text() == "")
    {
        Error *error = new Error(nullptr, "File name cannot be empty.");
        error->exec();
        pageTitle->setText(oldName);
        return;
    }

    QString newName = FileManager::renameFile(oldPath, pageTitle->text());
    SidebarManager::rename(index, newName);
    openFile(index);
}

/**
 * @brief DisplayManager::openFile parses all the blocks in a file
 * @param index
 */
void DisplayManager::openFile(QModelIndex index)
{
    //TODO fix this
    foreach (QWidget* widget, mainPage->findChildren<QWidget*>())   mainPage->layout()->removeWidget(widget);

    QString filePath = index.siblingAtColumn(1).data().toString();
    QFile file(filePath + "/files.mar");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Error *error = new Error(nullptr, "Error opening file.");
        error->exec();
        return;
    }

    QTextStream blocks(&file);



    while (!blocks.atEnd())
    {
        QFileInfo block(filePath + blocks.readLine());
        if (block.completeSuffix() == "html")
            addHtmlBlock(block.absoluteFilePath());
        else if (block.completeSuffix() == "mar")
            addSubfileBlock(block.absoluteFilePath());
    }

    file.close();

    pageTitle->setText(index.data().toString());
}

void DisplayManager::init(QFrame *mainPage, QLineEdit *pageTitle)
{
    DisplayManager::mainPage = mainPage;
    DisplayManager::pageTitle = pageTitle;
}
