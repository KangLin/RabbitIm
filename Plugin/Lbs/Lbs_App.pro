#-------------------------------------------------
#
# Project created by QtCreator 2015-08-02T13:43:29
#
#-------------------------------------------------

#RABBITIM_USE_LIBCURL=1      #使用 libcurl
#RABBITIM_USE_OPENSSL=1      #使用openssl

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

CONFIG += c++0x
!msvc{
    QMAKE_CXXFLAGS += " -std=c++0x "
}

TARGET = lbs
TEMPLATE = app

include(Lbs.pri)
SOURCES += main.cpp 
