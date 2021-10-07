#ifndef RICHTEXTFUNCTIONS_H
#define RICHTEXTFUNCTIONS_H
#include <QTextBrowser>
#include <QPushButton>


class RichTextFunctions
{
public:
    RichTextFunctions();
    static void boldText(QTextBrowser *block);
    static void italicText(QTextBrowser *block);
    static void underlineText(QTextBrowser *block);
    static QPushButton *boldButton;
    static QPushButton *italicButton;
    static QPushButton *underlineButton;
    static void init(QPushButton *boldButton, QPushButton *italicButton, QPushButton *underlineButton);
    static void toggleButton(QPushButton *button, bool active);
};

#endif // RICHTEXTFUNCTIONS_H
