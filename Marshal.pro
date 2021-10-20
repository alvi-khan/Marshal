QT       += core gui sql concurrent

RC_ICONS = Logo.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addressbar.cpp \
    blocks.cpp \
    calendar.cpp \
    calendarcontrols.cpp \
    calendarevent.cpp \
    databasemanager.cpp \
    displaymanager.cpp \
    error.cpp \
    eventdialog.cpp \
    filemanager.cpp \
    filesharedialog.cpp \
    handleexternalfile.cpp \
    linkeditdialog.cpp \
    logindialog.cpp \
    main.cpp \
    mainpage.cpp \
    mainwindow.cpp \
    reminder.cpp \
    reminderscontainer.cpp \
    richtextfunctions.cpp \
    textblock.cpp \
    settingsdialog.cpp \
    sidebarmanager.cpp

HEADERS += \
    addressbar.h \
    blocks.h \
    calendar.h \
    calendarcontrols.h \
    calendarevent.h \
    databasemanager.h \
    displaymanager.h \
    error.h \
    eventdialog.h \
    filemanager.h \
    filesharedialog.h \
    handleexternalfile.h \
    linkeditdialog.h \
    logindialog.h \
    mainpage.h \
    mainwindow.h \
    reminder.h \
    reminderscontainer.h \
    richtextfunctions.h \
    textblock.h \
    settingsdialog.h \
    sidebarmanager.h

FORMS += \
    addressbar.ui \
    calendarevent.ui \
    error.ui \
    eventdialog.ui \
    filesharedialog.ui \
    linkeditdialog.ui \
    logindialog.ui \
    mainwindow.ui \
    reminder.ui \
    reminderscontainer.ui \
    settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
