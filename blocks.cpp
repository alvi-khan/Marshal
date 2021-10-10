#include "blocks.h"
#include "calendar.h"
#include "displaymanager.h"
#include "error.h"
#include "filemanager.h"
#include "linkeditdialog.h"
#include "sidebarmanager.h"
#include "textblock.h"
#include "richtextfunctions.h"

#include <QFile>
#include <QTextBrowser>
#include <QVBoxLayout>

QFrame* Blocks::mainPage;

Blocks::Blocks()
{

}

void Blocks::onCustomContextMenu(const QPoint &point)
{
    QWidget *block = (QWidget *) sender();

    QMenu menu;
    menu.setStyleSheet("QMenu{background-color: rgb(46, 46, 46); color: rgb(255, 255, 255); padding: 2px;}\
                        QMenu::item:selected{background-color: #3E3E3E; color: rgb(255, 255, 255); border-radius: 2px;}");

    QAction* del = new QAction("Delete", this);
    del->setIcon(QIcon(":/Toolbar Icons/Resources/Toolbar Icons/Trash (Context Menu).svg"));

    connect(del, &QAction::triggered, [=] {deleteBlock(block);});
    menu.addAction(del);

    menu.exec(QCursor::pos());
    del->deleteLater();
}

void Blocks::deleteBlock(QWidget *block)
{
    block->hide();

    if (QLatin1String(block->metaObject()->className()) == "Calendar")
    {
        Calendar *calendar = (Calendar *) block;
        QString calendarPath = calendar->selfPath;
        QString parentPath = calendarPath.section("/", 0, -3);

        FileManager::deleteDirectory(calendarPath.section("/", 0, -2));
        FileManager::updateFileTracker(parentPath + "/files.mar", calendarPath.remove(parentPath), "");
        return;
    }


    QTextBrowser *htmlBlock = (QTextBrowser *) block;
    QString blockPath = htmlBlock->documentTitle();
    if (blockPath.endsWith("/files.mar"))       // subpage blocks
    {
        //cant use filemanager's delete page (removes wrong index problem)
        QString directory = blockPath.section("/", 0, -2);
        FileManager::deleteDirectory(directory);

        QModelIndex index = SidebarManager::getChild(directory);
        SidebarManager::removeItem(index);

        QString parentPage = blockPath.section("/", 0, -3);
        FileManager::updateFileTracker(parentPage + "/files.mar", blockPath.remove(parentPage), "");
    }
    else if (blockPath.endsWith(".html"))       // normal text blocks
    {
        QFile::remove(blockPath);

        QString parentPage = blockPath.section("/", 0, -2);
        FileManager::updateFileTracker(parentPage + "/files.mar", blockPath.remove(parentPage), "");
    }
    else if (blockPath.endsWith(".url"))
    {
        QFile::remove(blockPath);

        QString parentPage = blockPath.section("/", 0, -2);
        FileManager::updateFileTracker(parentPage + "/files.mar", blockPath.remove(parentPage), "");
    }
    // TODO delete calendar events
}

/**
 * @brief Blocks::createTextBrowser generates a basic text browser widget with custom formatting
 * @param content is the content to put in the widget
 * @return the text browser created
 */
QTextBrowser* Blocks::createTextBrowser(QString content)
{
    TextBlock *newBlock = new TextBlock();
    newBlock->insertHtml(content);

    // setting up style
    newBlock->setFrameStyle(QFrame::NoFrame);

    connect(newBlock, &QTextEdit::textChanged, new Blocks(), &Blocks::updateBlockSize);
    newBlock->textChanged();    // to trigger height readjustment
    connect(newBlock, &QTextEdit::textChanged, new FileManager(), &FileManager::saveBlock);
    connect(newBlock, &TextBlock::selectionChanged, [=] { RichTextFunctions::selectionChange(newBlock); });
    connect(newBlock, &QTextEdit::cursorPositionChanged, [=] { RichTextFunctions::selectionChange(newBlock); });

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
    htmlBlock->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(htmlBlock, SIGNAL(customContextMenuRequested(const QPoint &)), new Blocks(), SLOT(onCustomContextMenu(const QPoint &)));

    return htmlBlock;
}

void Blocks::addToolTip(QWidget *widget, QString text)
{
    widget->setToolTip(text);
    widget->setStyleSheet("QToolTip {border: 1px solid rgb(70, 70, 70);}");
}

/**
 * @brief Blocks::addLinkBlock adds a block with a link (internal/external)
 * @param link
 * @param name is the text displayed for the link
 */
QTextBrowser* Blocks::addLinkBlock(QString link, QString name)
{
    QString content = "<a style=\"color: #66d9ee\" href=\"" + link + "\">" + name + "</a>";
    QTextBrowser *linkBlock = createTextBrowser(content);
    linkBlock->setOpenLinks(false);
    linkBlock->setTextColor(QColor::fromRgb(102, 217, 238));
    linkBlock->connect(linkBlock, &QTextBrowser::anchorClicked, new DisplayManager(), &DisplayManager::openLink);
    linkBlock->setContextMenuPolicy(Qt::CustomContextMenu);


    if (!link.endsWith(".mar"))
    {
        connect(linkBlock, &QTextBrowser::customContextMenuRequested, new LinkEditDialog(linkBlock), &LinkEditDialog::displayDialog);
        addToolTip(linkBlock, link);
    }
    else
    {
        connect(linkBlock, SIGNAL(customContextMenuRequested(const QPoint &)), new Blocks(), SLOT(onCustomContextMenu(const QPoint &)));
        addToolTip(linkBlock, (link.remove("E:/Downloads/Main Folder/Private")).remove("/files.mar"));
    }
    return linkBlock;
}

/**
 * @brief Blocks::addSubfileBlock adds a block with a link to a subfile
 * @param filePath is the path to the subfile
 */
void Blocks::addSubfileBlock(QString filePath)
{
    QString fileName = filePath.section("/", -2, -2);
    QTextBrowser *subFileBlock = addLinkBlock(filePath, fileName);
    subFileBlock->setDocumentTitle(filePath);
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

    calendar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(calendar, SIGNAL(customContextMenuRequested(const QPoint &)), new Blocks(), SLOT(onCustomContextMenu(const QPoint &)));
}

void Blocks::init(QFrame *mainPage)
{
    Blocks::mainPage = mainPage;
}


