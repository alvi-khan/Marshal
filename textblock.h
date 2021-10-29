#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include <QTextBrowser>

/**
 * @brief The TextBlock class provides a basic text block with customized behaviour.
 */
class TextBlock : public QTextBrowser
{
public:
    TextBlock();
    void keyPressEvent(QKeyEvent *ev);
    void focusInEvent(QFocusEvent *e);
    void resizeEvent(QResizeEvent *event);
};

#endif // TEXTBLOCK_H
