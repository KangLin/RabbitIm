#-------------------------------------------------
#
# Project created by QtCreator 2015-12-12T22:30:12
#
#-------------------------------------------------

QT       += core gui

TARGET = Update
TEMPLATE = lib
CONFIG += plugin

RABBITIM_PLUG_NAME=CPluginUpdate

CONFIG += c++0x
!msvc{
    QMAKE_CXXFLAGS += " -std=c++0x "
}

include(../config.pri)

equals(RABBITIM_USE_LIBCURL, 1){
    SOURCES += PluginUpdate.cpp \
        $$PWD/DownLoad.cpp \
        $$PWD/DownLoadHandle.cpp \
        $$PWD/DlgUpdate.cpp

    HEADERS += PluginUpdate.h \
        $$PWD/DownLoad.h \
        $$PWD/DownLoadHandle.h \
        $$PWD/DlgUpdate.h

    FORMS += $$PWD/DlgUpdate.ui
    #RESOURCES += $$PWD/../../Resource/Resource.qrc
}

DISTFILES += Update.json

INCLUDEPATH += $$PWD/../..
