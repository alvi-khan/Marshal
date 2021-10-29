#include "textblock.h"
#include <QKeyEvent>
#include "blocks.h"
#include "mainwindow.h"

TextBlock::TextBlock()
{
    connect(this, &QTextEdit::textChanged, new Blocks(), &Blocks::updateBlockSize);
}

void TextBlock::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Backspace && this->toPlainText() == "") // remove empty block
        Blocks::deleteBlock(this);
    else
        QTextBrowser::keyPressEvent(ev);
}

void TextBlock::focusInEvent(QFocusEvent *e)
{
    if (e->gotFocus())  // remove selection from all other blocks
    {
        QList<QObject *> objects = MainWindow::window->mainPage->children();
        foreach (QObject *object, objects)
        {
            if (QLatin1String(object->metaObject()->className()) != "QTextBrowser")    continue;    // only for text blocks
            QTextBrowser *block = (QTextBrowser *) object;
            if (block == this)  continue;   // skip current block
            QTextCursor cursor = block->textCursor();
            cursor.clearSelection();
            block->setTextCursor(cursor);
        }
    }

   QTextBrowser::focusInEvent(e);
}

/**
 * @brief TextBlock::resizeEvent re-adjust block size
 */
void TextBlock::resizeEvent(QResizeEvent *event)
{
    this->textChanged();    // triggers re-calculation of block size
    QTextBrowser::resizeEvent(event);
}
