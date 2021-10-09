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
