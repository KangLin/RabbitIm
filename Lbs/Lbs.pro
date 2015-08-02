#-------------------------------------------------
#
# Project created by QtCreator 2015-08-02T13:43:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
 QT += positioning

TARGET = lbs
TEMPLATE = app

CONFIG += mobility
MOBILITY = 

include(Lbs.pri)
SOURCES += main.cpp
