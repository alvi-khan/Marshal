#include "blocks.h"
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

QTextBrowser* Blocks::createTextBrowser(QString content)
{
    QTextBrowser *newBlock = new QTextBrowser();
    newBlock->insertHtml(content);

    // setting up style
    newBlock->setFrameStyle(QFrame::NoFrame);
    newBlock->setTextColor(QColor::fromRgb(255, 255, 255));
    QFontMetrics m(newBlock->font());
    int rowHeight = m.lineSpacing();
    int lineCount = newBlock->document()->lineCount();
    newBlock->setMaximumHeight(rowHeight * lineCount * 1.5);

    // connect block to save function
    connect(newBlock, &QTextEdit::textChanged, new FileManager(), &FileManager::saveBlock);

    // add block to main page
    QVBoxLayout *layout = (QVBoxLayout *) mainPage->layout();
    layout->addWidget(newBlock);
    layout->setAlignment(Qt::AlignTop);
    mainPage->setLayout(layout);

    return newBlock;
}

QTextBrowser* Blocks::addHtmlBlock(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Error *error = new Error(nullptr, "Error opening file.");
        error->exec();
        return nullptr;
    }
    QTextStream content(&file);

    QTextBrowser *htmlBlock = createTextBrowser(content.readAll());

    file.close();

    htmlBlock->setDocumentTitle(filePath);
    htmlBlock->setReadOnly(false);          // TODO check if this is necessary
    return htmlBlock;
}

void Blocks::addLinkBlock(QString link, QString name)
{
    QString content = "<a style=\"color: #66d9ee\" href=\"" + link + "\">" + name + "</a>";
    QTextBrowser *linkBlock = createTextBrowser(content);
    linkBlock->setOpenLinks(false);
    linkBlock->setTextColor(QColor::fromRgb(102, 217, 238));
    linkBlock->connect(linkBlock, &QTextBrowser::anchorClicked, new DisplayManager(), &DisplayManager::openLink);
}

void Blocks::addSubfileBlock(QString filePath)
{
    QString fileName = filePath.section("/", -2, -2);
    addLinkBlock(filePath, fileName);
}

void Blocks::init(QFrame *mainPage)
{
    Blocks::mainPage = mainPage;
}
