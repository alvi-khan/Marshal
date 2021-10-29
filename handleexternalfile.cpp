#include "handleexternalfile.h"
#include <QFileDialog>
#include <QObject>
#include <displaymanager.h>
#include "filemanager.h"

HandleExternalFile::HandleExternalFile()
{

}
/**
 * @brief HandleExternalFile::addExternalFile prompts for file attachment and creates block with attachment
 */
void HandleExternalFile::addExternalFile()
{
    QString prompt = QObject::tr("Open File");
    QString startLocation = FileManager::homeDirectory;
    QString filters = QObject::tr("All Files(*)");

    QString filePath = QFileDialog::getOpenFileName(nullptr, prompt, startLocation, filters);
    QString fileName = filePath.section("/", -1);

    FileManager::createURLBlock(fileName, filePath);
}
