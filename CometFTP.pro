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
    SFTPSite.cpp \
    ServerFileModel.cpp \
    AddressBar.cpp \
    DownloadManager.cpp \
    CustomFileModel.cpp \
    About.cpp

HEADERS  += \
    MainWindow.h \
    SlidingStackedWidget.h \
    NcFramelessHelper.h \
    LocalExplorer.h \
    StatusArea.h \
    ServerExplorer.h \
    SFTPSite.h \
    ServerFileModel.h \
    AddressBar.h \
    DownloadManager.h \
    CustomFileModel.h \
    About.h

RESOURCES += \
    resources.qrc

CONFIG += c++11

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../Users/Shazib/Downloads/ -ldwmapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../Users/Shazib/Downloads/ -ldwmapi

INCLUDEPATH += $$PWD/../../../../../../Users/Shazib/Downloads
DEPENDPATH += $$PWD/../../../../../../Users/Shazib/Downloads

win32: LIBS += -ldwmapi

win32: LIBS += -lssh
win32: LIBS += -lssh_threads
win32: LIBS += -llibeay32
win32: LIBS += -lssleay32
