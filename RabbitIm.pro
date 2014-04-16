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

#android选项中包含了unix选项，所以在写工程如下条件判断时，必须把android条件放在unix条件前
android {
    INCLUDEPATH += $$PWD/ThirdLibary/android/include
    DEPENDPATH += $$PWD/ThirdLibary/android/include
    DEFINES += ANDROID
} else:win32 {
    INCLUDEPATH += $$PWD/ThirdLibary/windows/include
    DEPENDPATH += $$PWD/ThirdLibary/windows/include
    OPENCV_VERSION=300
} else:unix {
    INCLUDEPATH += $$PWD/ThirdLibary/unix/include
    DEPENDPATH += $$PWD/ThirdLibary/unix/include
}

FFMPEG_LIBRARY= -lavcodec -lavformat -lswscale -lswresample -lpostproc -lavfilter  -lavutil

OPENCV_LIBRARY=-lopencv_core$$OPENCV_VERSION \
    -lopencv_highgui$$OPENCV_VERSION \
    -lopencv_imgproc$$OPENCV_VERSION \
    -lopencv_cudaimgproc$$OPENCV_VERSION

android{
} else {
    OPENCV_LIBRARY += -llibjpeg  -llibwebp -llibtiff -llibpng -llibjasper -lIlmImf -lzlib
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ThirdLibary/windows/lib -l$$QXMPP_LIBRARY_NAME $$OPENCV_LIBRARY $$FFMPEG_LIBRARY
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ThirdLibary/windows/lib -l$${QXMPP_LIBRARY_NAME}0 $$OPENCV_LIBRARY $$FFMPEG_LIBRARY
else:android: LIBS += -L$$PWD/ThirdLibary/android/lib -l$$QXMPP_LIBRARY_NAME  $$OPENCV_LIBRARY  $$FFMPEG_LIBRARY
else:unix: LIBS += -L$$PWD/ThirdLibary/unix/lib -l$$QXMPP_LIBRARY_NAME $$OPENCV_LIBRARY $$FFMPEG_LIBRARY

QXMPP_USE_VPX=1
QXMPP_USE_SPEEX=1

!isEmpty(QXMPP_USE_SPEEX) {
    DEFINES += QXMPP_USE_SPEEX
    LIBS += -lspeex
}

!isEmpty(QXMPP_USE_THEORA) {
    DEFINES += QXMPP_USE_THEORA
    LIBS += -ltheoradec -ltheoraenc
}

!isEmpty(QXMPP_USE_VPX) {
    DEFINES += QXMPP_USE_VPX
    LIBS += -lvpx
    android {
        LIBS+= -lcpu-features
    }
}

DEFINES += __STDC_CONSTANT_MACROS #ffmpeg需要

include(RabbitIm.pri)

CONFIG += mobility

MOBILITY = 

CONFIG += localize_deployment #本地语言部署

TRANSLATIONS += \
    Resource/translations/app_zh_CN.ts

OTHER_FILES += README.md \
    .gitignore \
    Resource/translations/app_zh_CN.ts \
    docs/* \
    docs/Books/* \
    docs/QXmpp音视频呼叫流程.txt \
    ThirdLibary/build/build_android_ffmpeg.sh \
    ThirdLibary/build/build_android_speex.sh \
    ThirdLibary/build/build_android_x264.sh \
    ThirdLibary/build/build_windows_libvpx.sh


ANDROID_EXTRA_LIBS = 
