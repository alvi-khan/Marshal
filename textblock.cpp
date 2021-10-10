#include "textblock.h"
#include <QKeyEvent>
#include "blocks.h"

TextBlock::TextBlock()
{

}

void TextBlock::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Backspace && this->toPlainText() == "")
        Blocks::deleteBlock(this);
    else
        QTextBrowser::keyPressEvent(ev);
}

void TextBlock::focusOutEvent(QFocusEvent *e)
{
    if (e->lostFocus())
    {
        QTextCursor cursor = textCursor();
        cursor.clearSelection();
        setTextCursor(cursor);
    }

   QTextEdit::focusOutEvent(e);
}
