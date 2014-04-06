#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

QT       += core gui network xml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RabbitIm
TEMPLATE = app

# Determine library name
CONFIG(debug, debug|release) {
    QXMPP_LIBRARY_NAME = qxmpp_d
} else {
    QXMPP_LIBRARY_NAME = qxmpp
}

FFMPEG_LIBRARY= -lavcodec -lavformat -lavutil -lswscale -lswresample -lpostproc -lavfilter

android {
    INCLUDEPATH += $$PWD/ThirdLibary/android/include
    DEPENDPATH += $$PWD/ThirdLibary/android/include
} else:win32 {
    INCLUDEPATH += $$PWD/ThirdLibary/windows/include
    DEPENDPATH += $$PWD/ThirdLibary/windows/include
} else:unix {
    INCLUDEPATH += $$PWD/ThirdLibary/linux/include
    DEPENDPATH += $$PWD/ThirdLibary/linux/include
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ThirdLibary/windows/lib -l$$QXMPP_LIBRARY_NAME $$FFMPEG_LIBRARY
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ThirdLibary/windows/lib -l$${QXMPP_LIBRARY_NAME}0 $$FFMPEG_LIBRARY
else:unix: LIBS += -L$$PWD/ThirdLibary/android/lib -l$$QXMPP_LIBRARY_NAME $$FFMPEG_LIBRARY
else:android: LIBS += -L$$PWD/ThirdLibary/android/lib -l$$QXMPP_LIBRARY_NAME $$FFMPEG_LIBRARY

DEFINES += __STDC_CONSTANT_MACROS

include(RabbitIm.pri)

CONFIG += mobility

MOBILITY = 

CONFIG += localize_deployment #本地语言部署

TRANSLATIONS += \
    translations/app_zh_CN.ts

OTHER_FILES += README.md \
    .gitignore \
    translations/app_zh_CN.ts \
    docs/* \
    docs/Books/* \
    docs/QXmpp音视频呼叫流程.txt

ANDROID_EXTRA_LIBS = 
