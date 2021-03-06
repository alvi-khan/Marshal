#include "filemanager.h"
#include "linkeditdialog.h"
#include "ui_linkeditdialog.h"

#include <QTextBrowser>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include "blocks.h"

LinkEditDialog::LinkEditDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkEditDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    effect->setXOffset(5);
    effect->setYOffset(5);
    effect->setColor(QColor("#1E1E1E"));
    this->ui->frame->setGraphicsEffect(effect);
}

LinkEditDialog::~LinkEditDialog()
{
    delete ui;
}

void LinkEditDialog::displayDialog(QPoint point)
{
    linkBlock = qobject_cast<QTextBrowser*>(sender());
    text = linkBlock->toPlainText();
    link = linkBlock->anchorAt(point);

    this->ui->fileName->setText(text);
    this->ui->filePath->setText(link);

    QPoint position = linkBlock->mapToGlobal(point);
    this->move(position);
    this->show();
}

/**
 * @brief LinkEditDialog::on_fileName_editingFinished updates file name on local storage
 */
void LinkEditDialog::on_fileName_editingFinished()
{
    QString newText = this->ui->fileName->text();
    if (newText == "")
    {
        this->ui->fileName->setText(text);
        return;
    }

    QString parentFile = FileManager::openFile;
    QString oldPath = text + ".url";
    QString newPath = newText + ".url";

    // update file and parent's tracker
    FileManager::renameFile(parentFile + "/" + oldPath, newPath);
    FileManager::updateFileTracker(parentFile + "/files.mar", "/" + oldPath, "/" + newPath);

    QString content = "<a style=\"color: #66d9ee\" href=\"" + link + "\">" + newText + "</a>";
    linkBlock->setHtml(content);
    linkBlock->setToolTip(link);

    text = newText;
}

/**
 * @brief LinkEditDialog::on_filePath_editingFinished updates link on locally stored file
 */
void LinkEditDialog::on_filePath_editingFinished()
{
    QString newLink = this->ui->filePath->text();
    if (newLink == "")
    {
        this->ui->filePath->setText(link);
        return;
    }

    // update file
    QString urlFile = FileManager::openFile + "/" + text + ".url";
    FileManager::writeToFile(urlFile, newLink);

    QString content = "<a style=\"color: #66d9ee\" href=\"" + newLink + "\">" + text + "</a>";
    linkBlock->setHtml(content);
    linkBlock->setToolTip(newLink);

    link = newLink;
}

/**
 * @brief LinkEditDialog::on_trashButton_clicked deletes attachment block
 */
void LinkEditDialog::on_trashButton_clicked()
{
    this->hide();
    Blocks::deleteBlock(this->parentWidget());
}

