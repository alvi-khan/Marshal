#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include <QTextBrowser>

class TextBlock : public QTextBrowser
{
public:
    TextBlock();
    void keyPressEvent(QKeyEvent *ev);
    void focusOutEvent(QFocusEvent *e);
};

#endif // TEXTBLOCK_H
