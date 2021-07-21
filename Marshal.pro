QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    displaymanager.cpp \
    error.cpp \
    filemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    sidebarmanager.cpp

HEADERS += \
    displaymanager.h \
    error.h \
    filemanager.h \
    mainwindow.h \
    sidebarmanager.h

FORMS += \
    error.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
