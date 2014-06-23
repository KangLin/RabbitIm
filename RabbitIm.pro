#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

QT += core gui network xml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RabbitIm
TEMPLATE = app

#equals(RABBITIM_LIBRARY_TYPE, shared) {
#    CONFIG += shared    #生成动态库
#} else {
#    CONFIG += staticlib #生成静态库
#}

# qxmpp 库名
CONFIG(debug, debug|release) {
    QXMPP_LIBRARY_NAME = qxmpp_d
    #调试宏
     DEFINES += DEBUG DEBUG_VIDEO_TIME 
} else {
    QXMPP_LIBRARY_NAME = qxmpp
}

QXMPP_USE_VPX = 1
#QXMPP_USE_SPEEX=1

!isEmpty(QXMPP_USE_SPEEX) {
    CODEC_LIBRARY += -lspeex
}

!isEmpty(QXMPP_USE_VPX) {
    CODEC_LIBRARY += -lvpx
    android {
        CODEC_LIBRARY += -lcpu-features
    }
}

FFMPEG_LIBRARY= -lavcodec -lavformat -lswscale -lswresample -lavfilter  -lavutil

#android选项中包含了unix选项，所以在写工程如下条件判断时，必须把android条件放在unix条件前
android{
    INCLUDEPATH += $$PWD/ThirdLibary/android/include $$WEBRTC_ROOT
    DEPENDPATH += $$PWD/ThirdLibary/android/include $$WEBRTC_ROOT
    DEFINES += ANDROID

    LIBS += -L$$PWD/ThirdLibary/android/lib  
} else:win32{
    INCLUDEPATH += $$PWD/ThirdLibary/windows/include $$WEBRTC_ROOT
    DEPENDPATH += $$PWD/ThirdLibary/windows/include $$WEBRTC_ROOT

    msvc {
        LDFLAGS += -ladvapi32
    }

    CONFIG(release, debug|release){
        msvc{
            LDFLAGS += /NODEFAULTLIB:libcmt
        }

        OPENCV_VERSION=300
    } else:CONFIG(debug, debug|release){
        DEFINES += DEBUG DEBUG_VIDEO_TIME

        msvc{
            LDFLAGS += /NODEFAULTLIB:libcmtd /NODEFAULTLIB:libcmt
        }

        OPENCV_VERSION=300d
    }

    WEBRTC_LIBRARY_DIR = .
    #WEBRTC_LIBRARY = -L$$WEBRTC_LIBRARY_DIR -llibjingle -llibjingle_media -llibjingle_p2p -lwebrtc

    LIBS += -L$$PWD/ThirdLibary/windows/lib $$LDFLAGS
} else:unix {
    INCLUDEPATH += $$PWD/ThirdLibary/unix/include $$WEBRTC_ROOT
    DEPENDPATH += $$PWD/ThirdLibary/unix/include $$WEBRTC_ROOT

    LIBS += -L$$PWD/ThirdLibary/unix/lib 
}

!isEmpty(RABBITIM_USER_OPENCV) {
    DEFINES += RABBITIM_USER_OPENCV
    OPENCV_LIBRARY=-lopencv_core$$OPENCV_VERSION \
    -lopencv_imgproc$$OPENCV_VERSION
}

LIBS += -l$$QXMPP_LIBRARY_NAME $$WEBRTC_LIBRARY $$OPENCV_LIBRARY $$FFMPEG_LIBRARY $$CODEC_LIBRARY

DEFINES += __STDC_CONSTANT_MACROS #ffmpeg需要

include(RabbitIm.pri)

CONFIG += mobility

MOBILITY = 

#应用程序图标
RC_FILE = AppIcon.rc

CONFIG += localize_deployment  #本地语言部署

TRANSLATIONS += \
    Resource/translations/app_zh_CN.ts

#安装
#INSTALLS=target

OTHER_FILES += README.md \
    .gitignore \
    AppIcon.rc \
    Resource/translations/app_zh_CN.ts \
    docs/* \
    docs/Books/* \
    docs/QXmpp音视频呼叫流程.txt \
    ThirdLibary/build_script/*.sh

ANDROID_EXTRA_LIBS = 
