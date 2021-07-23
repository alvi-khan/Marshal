#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QFrame>
#include <QDebug>



class MainPage : public QFrame
{
    Q_OBJECT
public:
    MainPage();
    MainPage(QWidget *&);
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
};

#endif // MAINPAGE_H
