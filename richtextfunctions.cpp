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

void RichTextFunctions::italicText(QTextBrowser *block)
{
    QFont font = block->currentFont();
    font.italic() ? font.setItalic(false) : font.setItalic(true);
    block->setCurrentFont(font);
    block->setFocus();
}

void RichTextFunctions::underlineText(QTextBrowser *block)
{
    QFont font = block->currentFont();
    font.underline() ? font.setUnderline(false) : font.setUnderline(true);
    block->setCurrentFont(font);
    block->setFocus();
}
