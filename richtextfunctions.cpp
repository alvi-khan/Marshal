#include "richtextfunctions.h"
#include <QPushButton>

QPushButton * RichTextFunctions::boldButton;
QPushButton * RichTextFunctions::italicButton;
QPushButton * RichTextFunctions::underlineButton;

RichTextFunctions::RichTextFunctions()
{

}

void RichTextFunctions::init(QPushButton *boldButton, QPushButton *italicButton, QPushButton *underlineButton)
{
    RichTextFunctions::boldButton = boldButton;
    RichTextFunctions::italicButton = italicButton;
    RichTextFunctions::underlineButton = underlineButton;
}

void RichTextFunctions::boldText(QTextBrowser *block)
{
    QFont font = block->currentFont();

    if (font.bold())
    {
        font.setBold(false);
        boldButton->setStyleSheet("background-color: rgb(46,46,46); padding: 5px;");
    }
    else
    {
        font.setBold(true);
        boldButton->setStyleSheet("background-color: #444755; padding: 5px;");
    }

    block->setCurrentFont(font);
    block->setFocus();
}

void RichTextFunctions::italicText(QTextBrowser *block)
{
    QFont font = block->currentFont();

    if (font.italic())
    {
        font.setItalic(false);
        italicButton->setStyleSheet("background-color: rgb(46,46,46); padding: 5px;");
    }
    else
    {
        font.setItalic(true);
        italicButton->setStyleSheet("background-color: #444755; padding: 5px;");
    }

    block->setCurrentFont(font);
    block->setFocus();
}

void RichTextFunctions::underlineText(QTextBrowser *block)
{
    QFont font = block->currentFont();

    if (font.underline())
    {
        font.setUnderline(false);
        underlineButton->setStyleSheet("background-color: rgb(46,46,46); padding: 5px;");
    }
    else
    {
        font.setUnderline(true);
        underlineButton->setStyleSheet("background-color: #444755; padding: 5px;");
    }

    block->setCurrentFont(font);
    block->setFocus();
}
