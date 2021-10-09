#include "mainpage.h"
#include "filemanager.h"
#include "sidebarmanager.h"
#include <QMouseEvent>

MainPage::MainPage()
{

}

MainPage::MainPage(QWidget *&)
{

}

/**
 * @brief MainPage::mousePressEvent creates a new empty block in the widget
 * @param event
 */
void MainPage::mousePressEvent(QMouseEvent *event)
{
    // create an empty block in the currently open page
    if (event->button() == Qt::LeftButton)
        FileManager::createBlock(SidebarManager::getCurrentIndex());

    // perform default functionality
    QFrame::mousePressEvent(event);
}
