#include "textblock.h"
#include <QKeyEvent>
#include "blocks.h"
#include "mainwindow.h"

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

void TextBlock::focusInEvent(QFocusEvent *e)
{
    if (e->gotFocus())
    {
        QList<QObject *> objects = MainWindow::window->mainPage->children();
        foreach (QObject *object, objects)
        {
            if (QLatin1String(object->metaObject()->className()) != "QTextBrowser")    continue;
            QTextBrowser *block = (QTextBrowser *) object;
            if (block == this)  continue;
            QTextCursor cursor = block->textCursor();
            cursor.clearSelection();
            block->setTextCursor(cursor);
        }
    }

   QTextBrowser::focusInEvent(e);
}
