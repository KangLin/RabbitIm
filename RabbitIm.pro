#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RabbitIm
TEMPLATE = app

# Determine library name
CONFIG(debug, debug|release) {
    QXMPP_LIBRARY_NAME = qxmpp_d
} else {
    QXMPP_LIBRARY_NAME = qxmpp
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ThirdLibary/lib/ -l$$QXMPP_LIBRARY_NAME
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ThirdLibary/lib/ -l$${QXMPP_LIBRARY_NAME}0
else:unix: LIBS += -L$$PWD/ThirdLibary/lib/ -l$$QXMPP_LIBRARY_NAME

INCLUDEPATH += $$PWD/ThirdLibary/include
DEPENDPATH += $$PWD/ThirdLibary/include

include(RabbitIm.pri)

CONFIG += mobility

MOBILITY = 

CONFIG += localize_deployment #本地语言部署

TRANSLATIONS += \
    translations/app_zh_CN.ts

OTHER_FILES += README.md \
    .gitignore \
    translations/app_zh_CN.ts

