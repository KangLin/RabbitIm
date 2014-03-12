#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RabbitIm
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    Widgets/DlgLogin/DlgLogin.cpp \
    Widgets/DlgLogin/FrmLogin.cpp

HEADERS  += MainWindow.h \
    Widgets/DlgLogin/DlgLogin.h \
    Widgets/DlgLogin/FrmLogin.h

FORMS    += MainWindow.ui \
    Widgets/DlgLogin/DlgLogin.ui \
    Widgets/DlgLogin/FrmLogin.ui

CONFIG += mobility
MOBILITY = 

TRANSLATIONS += \
    translations/app_zh_CN.ts

OTHER_FILES += README.md \
    .gitignore \
    translations/app_zh_CN.ts
