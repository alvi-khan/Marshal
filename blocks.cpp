#include "blocks.h"
#include "calendar.h"
#include "displaymanager.h"
#include "error.h"
#include "filemanager.h"

#include <QFile>
#include <QTextBrowser>
#include <QVBoxLayout>

QFrame* Blocks::mainPage;

Blocks::Blocks()
{

}

/**
 * @brief Blocks::createTextBrowser generates a basic text browser widget with custom formatting
 * @param content is the content to put in the widget
 * @return the text browser created
 */
QTextBrowser* Blocks::createTextBrowser(QString content)
{
    QTextBrowser *newBlock = new QTextBrowser();
    newBlock->insertHtml(content);

    // setting up style
    newBlock->setFrameStyle(QFrame::NoFrame);
    newBlock->setTextColor(QColor::fromRgb(255, 255, 255)); // white
    //newBlock->setFontPointSize(12);
    QFontMetrics m(newBlock->font());
    int rowHeight = m.lineSpacing();
    int lineCount = newBlock->document()->lineCount() + 1;
    newBlock->setMaximumHeight(rowHeight * lineCount);
    newBlock->setMinimumHeight(rowHeight * lineCount);

    // connect block to save function
    connect(newBlock, &QTextEdit::textChanged, new FileManager(), &FileManager::saveBlock);

    // connect block to update function
    connect(newBlock, &QTextEdit::textChanged, new Blocks(), &Blocks::updateBlockSize);

    // add block to main page
    QVBoxLayout *layout = (QVBoxLayout *) mainPage->layout();
    layout->insertWidget(layout->count() - 1, newBlock);    // add above spacer
    layout->setAlignment(Qt::AlignTop);
    mainPage->setLayout(layout);

    return newBlock;
}

void Blocks::updateBlockSize()
{
    QTextBrowser *htmlBlock = qobject_cast<QTextBrowser*>(sender());
    QFontMetrics m(htmlBlock->font());
    int rowHeight = m.lineSpacing();
    int lineCount = htmlBlock->document()->lineCount() + 1;
    htmlBlock->setMaximumHeight(rowHeight * lineCount);
    htmlBlock->setMinimumHeight(rowHeight * lineCount);
}

/**
 * @brief Blocks::addHtmlBlock adds a block with text from a specific file
 * @param filePath is the path to the file
 * @return the block created
 */
QTextBrowser* Blocks::addHtmlBlock(QString filePath)
{
    QString data = FileManager::readFromFile(filePath);

    QTextBrowser *htmlBlock = createTextBrowser(data);
    htmlBlock->setDocumentTitle(filePath);  // storing filepath in hidden title; needed to save block
    htmlBlock->setReadOnly(false);          // TODO check if this is necessary
    return htmlBlock;
}

/**
 * @brief Blocks::addLinkBlock adds a block with a link (internal/external)
 * @param link
 * @param name is the text displayed for the link
 */
void Blocks::addLinkBlock(QString link, QString name)
{
    QString content = "<a style=\"color: #66d9ee\" href=\"" + link + "\">" + name + "</a>";
    QTextBrowser *linkBlock = createTextBrowser(content);
    linkBlock->setOpenLinks(false);
    linkBlock->setTextColor(QColor::fromRgb(102, 217, 238));
    linkBlock->connect(linkBlock, &QTextBrowser::anchorClicked, new DisplayManager(), &DisplayManager::openLink);
}

/**
 * @brief Blocks::addSubfileBlock adds a block with a link to a subfile
 * @param filePath is the path to the subfile
 */
void Blocks::addSubfileBlock(QString filePath)
{
    QString fileName = filePath.section("/", -2, -2);
    addLinkBlock(filePath, fileName);
}

void Blocks::addCalendarBlock(QString filePath)
{
    Calendar *calendar = new Calendar(filePath);
    QVBoxLayout *layout = (QVBoxLayout *) mainPage->layout();
    layout->insertWidget(layout->count() - 1, calendar);
}

void Blocks::init(QFrame *mainPage)
{
    Blocks::mainPage = mainPage;
}


