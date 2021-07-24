#ifndef RICHTEXTFUNCTIONS_H
#define RICHTEXTFUNCTIONS_H
#include <QTextBrowser>


class RichTextFunctions
{
public:
    RichTextFunctions();
    static void boldText(QTextBrowser *block);
    static void italicText(QTextBrowser *block);
    static void underlineText(QTextBrowser *block);

};

#endif // RICHTEXTFUNCTIONS_H
