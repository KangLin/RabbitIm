#-------------------------------------------------
#
# Project created by QtCreator 2015-12-18T13:15:27
#
#-------------------------------------------------


QT *= core

TARGET = PluginConverFormatFFMpeg
TEMPLATE = lib

OTHER_FILES += CMakeLists.txt

PLUGIN_TYPE=ConverFormat
RABBITIM_PLUG_NAME=PluginConverFormatFFMpeg
include(../../pri/ConfigCommon.pri)

SOURCES += ConverFormatOpenCV.cpp

HEADERS += ConverFormatOpenCV.h
