#include "blocks.h"
#include "calendar.h"
#include "displaymanager.h"
#include "error.h"
#include "filemanager.h"
#include "linkeditdialog.h"

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

    connect(newBlock, &QTextEdit::textChanged, new Blocks(), &Blocks::updateBlockSize);
    newBlock->textChanged();    // to trigger height readjustment
    connect(newBlock, &QTextEdit::textChanged, new FileManager(), &FileManager::saveBlock);

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

void Blocks::addToolTip(QWidget *widget, QString text)
{
    widget->setToolTip(text);
    widget->setStyleSheet("QToolTip {\
                            color: #ffffff; \
                            background-color: #000000; \
                            border: 0px; }");
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
    addToolTip(linkBlock, link);


    if (!link.endsWith(".mar"))
    {
        linkBlock->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(linkBlock, &QTextBrowser::customContextMenuRequested, new LinkEditDialog(), &LinkEditDialog::displayDialog);
    }
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
    QHBoxLayout *layout1 = new QHBoxLayout();
    layout1->addWidget(calendar);
    layout1->setAlignment(Qt::AlignHCenter);
    QWidget *widget = new QWidget();
    widget->setLayout(layout1);
    QVBoxLayout *layout = (QVBoxLayout *) mainPage->layout();
    layout->insertWidget(layout->count() - 1, widget);
}

void Blocks::init(QFrame *mainPage)
{
    Blocks::mainPage = mainPage;
}


