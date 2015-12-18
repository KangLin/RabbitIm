#-------------------------------------------------
#
# Project created by QtCreator 2015-12-11T15:35:40
#
#-------------------------------------------------

QT *= core gui widgets

TARGET = ScreenShot
TEMPLATE = lib
DISTFILES += PluginScreenShot.json

RABBITIM_PLUG_NAME=CPluginScreenShot

include(../ConfigApp.pri)

SOURCES += PluginScreenShot.cpp \
    WdgScreenEditor.cpp \
    DlgScreenShot.cpp

HEADERS += PluginScreenShot.h \
    WdgScreenEditor.h \
    DlgScreenShot.h

#RESOURCES += $$PWD/../../Resource/Resource.qrc #资源已在libRabbitIm中存在


