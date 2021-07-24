#include "richtextfunctions.h"

RichTextFunctions::RichTextFunctions()
{

}

void RichTextFunctions::boldText(QTextBrowser *block)
{
    QFont font = block->currentFont();
    font.bold() ? font.setBold(false) : font.setBold(true);
    block->setCurrentFont(font);
    block->setFocus();
}
