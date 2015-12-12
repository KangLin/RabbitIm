#-------------------------------------------------
#
# Project created by QtCreator 2015-12-11T15:35:40
#
#-------------------------------------------------

QT += core gui widgets 

TARGET = ScreenShot
TEMPLATE = lib
CONFIG += plugin

RABBITIM_PLUG_NAME=CPluginScreenShot

CONFIG += c++0x
!msvc{
    QMAKE_CXXFLAGS += " -std=c++0x "
}

include(../config.pri)

SOURCES += PluginScreenShot.cpp \
    WdgScreenEditor.cpp \
    DlgScreenShot.cpp

HEADERS += PluginScreenShot.h \
    WdgScreenEditor.h \
    DlgScreenShot.h

#RESOURCES += $$PWD/../../Resource/Resource.qrc #资源已在libRabbitIm中存在

DISTFILES += PluginScreenShot.json

INCLUDEPATH += $$PWD/../..
