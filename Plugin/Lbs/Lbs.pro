#-------------------------------------------------
#
# Project created by QtCreator 2015-08-02T13:43:29
#
#-------------------------------------------------

#RABBITIM_USE_LIBCURL=1      #使用 libcurl
#RABBITIM_USE_OPENSSL=1      #使用openssl

TARGET = Lbs
TEMPLATE = lib

CONFIG += mobility
MOBILITY = 

RABBITIM_PLUG_NAME=CPluginAppMotion
include(Lbs.pri)

DISTFILES += Lbs.json
