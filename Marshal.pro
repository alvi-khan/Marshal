QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    blocks.cpp \
    calendar.cpp \
    calendarcontrols.cpp \
    calendarevent.cpp \
    databasemanager.cpp \
    displaymanager.cpp \
    error.cpp \
    filemanager.cpp \
    handleexternalfile.cpp \
    logindialog.cpp \
    main.cpp \
    mainpage.cpp \
    mainwindow.cpp \
    newevent.cpp \
    richtextfunctions.cpp \
    sidebarmanager.cpp

HEADERS += \
    blocks.h \
    calendar.h \
    calendarcontrols.h \
    calendarevent.h \
    databasemanager.h \
    displaymanager.h \
    error.h \
    filemanager.h \
    handleexternalfile.h \
    logindialog.h \
    mainpage.h \
    mainwindow.h \
    newevent.h \
    richtextfunctions.h \
    sidebarmanager.h

FORMS += \
    error.ui \
    logindialog.ui \
    mainwindow.ui \
    newevent.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
