#include "handleexternalfile.h"
#include <QFileDialog>
#include <QObject>
#include <displaymanager.h>
#include "filemanager.h"

HandleExternalFile::HandleExternalFile()
{

}

void HandleExternalFile::addExternalFile()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open File"), "/home", QObject::tr("PDF (*.pdf);;DOC Files (*.docx)"));
    qDebug()<<filePath;
    QString fileName = filePath.section("/", -1);
    FileManager::createURLBlock(fileName, filePath);
}
