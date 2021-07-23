#include "mainpage.h"
#include "filemanager.h"
#include "sidebarmanager.h"

MainPage::MainPage()
{

}

MainPage::MainPage(QWidget *&)
{

}

void MainPage::mousePressEvent(QMouseEvent *event)
{
    FileManager::createBlock(SidebarManager::getCurrentIndex());
    QFrame::mousePressEvent(event);
}
