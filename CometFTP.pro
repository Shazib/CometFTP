#-------------------------------------------------
#
# Project created by QtCreator 2014-04-15T20:51:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CometFTP
TEMPLATE = app


SOURCES += main.cpp\
    MainWindow.cpp \
    SlidingStackedWidget.cpp \
    NcFramlessHelper.cpp \
    LocalExplorer.cpp \
    StatusArea.cpp \
    ServerExplorer.cpp \
    SFTPSite.cpp

HEADERS  += \
    MainWindow.h \
    SlidingStackedWidget.h \
    NcFramelessHelper.h \
    LocalExplorer.h \
    StatusArea.h \
    ServerExplorer.h \
    SFTPSite.h

RESOURCES += \
    resources.qrc
