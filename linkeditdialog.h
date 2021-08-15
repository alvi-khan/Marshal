#ifndef LINKEDITDIALOG_H
#define LINKEDITDIALOG_H

#include <QWidget>
#include <QTextBrowser>
#include "mainwindow.h"

namespace Ui {
class LinkEditDialog;
}

class LinkEditDialog : public QWidget
{
    Q_OBJECT

public:
    explicit LinkEditDialog(QWidget *parent = nullptr);
    ~LinkEditDialog();

public slots:
    void displayDialog(QPoint point);
private slots:
    void on_fileName_editingFinished();

    void on_filePath_editingFinished();

private:
    Ui::LinkEditDialog *ui;
    QTextBrowser *linkBlock;
    QString text, link;
};

#endif // LINKEDITDIALOG_H
