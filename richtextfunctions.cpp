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

void RichTextFunctions::toggleButton(QPushButton *button, bool active)
{
    if(active)
        button->setStyleSheet("background-color: #4E4D4D; padding: 10px;");
    else
        button->setStyleSheet("QPushButton:hover{background-color: #3E3E3E;}"
                              "*{background-color: rgb(47, 52, 55); padding: 10px;}");
}

void RichTextFunctions::boldText(QTextBrowser *block)
{
    QFont font = block->currentFont();

    bool status = font.bold();
    font.setBold(!status);
    toggleButton(boldButton, !status);

    block->setCurrentFont(font);
    block->setFocus();
}

void RichTextFunctions::italicText(QTextBrowser *block)
{
    QFont font = block->currentFont();

    bool status = font.italic();
    font.setItalic(!status);
    toggleButton(italicButton, !status);

    block->setCurrentFont(font);
    block->setFocus();
}

void RichTextFunctions::underlineText(QTextBrowser *block)
{
    QFont font = block->currentFont();

    bool status = font.underline();
    font.setUnderline(!status);
    toggleButton(underlineButton, !status);

    block->setCurrentFont(font);
    block->setFocus();
}
