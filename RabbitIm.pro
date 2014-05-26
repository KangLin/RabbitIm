#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

QT += core gui network xml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RabbitIm
TEMPLATE = app

# qxmpp 库名
CONFIG(debug, debug|release) {
    QXMPP_LIBRARY_NAME = qxmpp_d
} else {
    QXMPP_LIBRARY_NAME = qxmpp
}

QXMPP_USE_VPX=1
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

win32{
    CONFIG(debug, debug|release){
        OPENCV_VERSION=300d
    } else {
        OPENCV_VERSION=300
    }
}
OPENCV_LIBRARY=-lopencv_core$$OPENCV_VERSION \
    -lopencv_imgproc$$OPENCV_VERSION

#WEBRTC_LIBRARY = -L$$WEBRTC_LIBRARY_DIR -ljingle

FFMPEG_LIBRARY= -lavcodec -lavformat -lswscale -lswresample -lavfilter  -lavutil

#android选项中包含了unix选项，所以在写工程如下条件判断时，必须把android条件放在unix条件前
android{
    INCLUDEPATH += $$PWD/ThirdLibary/android/include $$WEBRTC_ROOT
    DEPENDPATH += $$PWD/ThirdLibary/android/include $$WEBRTC_ROOT
    DEFINES += ANDROID

    CONFIG(release, debug|release){
        LIBS += -L$$PWD/ThirdLibary/android/lib -l$$QXMPP_LIBRARY_NAME $$WEBRTC_LIBRARY $$OPENCV_LIBRARY $$CODEC_LIBRARY $$FFMPEG_LIBRARY
    } else:CONFIG(debug, debug|release){
        DEFINES += DEBUG DEBUG_VIDEO_TIME
        LIBS += -L$$PWD/ThirdLibary/android/lib -l$$QXMPP_LIBRARY_NAME $$WEBRTC_LIBRARY $$OPENCV_LIBRARY $$CODEC_LIBRARY $$FFMPEG_LIBRARY
    }
} else:win32{
    INCLUDEPATH += $$PWD/ThirdLibary/windows/include $$WEBRTC_ROOT
    DEPENDPATH += $$PWD/ThirdLibary/windows/include $$WEBRTC_ROOT

    msvc {
        LDFLAGS += -ladvapi32
    }

    CONFIG(release, debug|release){
        LIBS += -L$$PWD/ThirdLibary/windows/lib -l$${QXMPP_LIBRARY_NAME}0 $$WEBRTC_LIBRARY $$OPENCV_LIBRARY $$FFMPEG_LIBRARY $$LDFLAGS
    } else:CONFIG(debug, debug|release){
        DEFINES += DEBUG DEBUG_VIDEO_TIME

        msvc{
            LDFLAGS += /NODEFAULTLIB:libcmt
        }

        LIBS += -L$$PWD/ThirdLibary/windows/lib -l$${QXMPP_LIBRARY_NAME}0 $$WEBRTC_LIBRARY $$OPENCV_LIBRARY $$FFMPEG_LIBRARY $$LDFLAGS
    }
} else:unix {
    INCLUDEPATH += $$PWD/ThirdLibary/unix/include $$WEBRTC_ROOT
    DEPENDPATH += $$PWD/ThirdLibary/unix/include $$WEBRTC_ROOT

    LIBS += -L$$PWD/ThirdLibary/unix/lib -l$$QXMPP_LIBRARY_NAME $$WEBRTC_LIBRARY $$OPENCV_LIBRARY $$FFMPEG_LIBRARY
}

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
INSTALLS=target

OTHER_FILES += README.md \
    .gitignore \
    AppIcon.rc \
    Resource/translations/app_zh_CN.ts \
    docs/* \
    docs/Books/* \
    docs/QXmpp音视频呼叫流程.txt \
    ThirdLibary/build/build_android_ffmpeg.sh \
    ThirdLibary/build/build_android_speex.sh \
    ThirdLibary/build/build_andriod_speexdsp.sh \
    ThirdLibary/build/build_android_x264.sh \
    ThirdLibary/build/build_android_libvpx.sh \
    ThirdLibary/build/build_windows_ffmpeg_mingw.sh \
    ThirdLibary/build/build_windows_ffmpeg_msvc.sh \
    ThirdLibary/build/build_windows_libvpx_msvc.sh \
    ThirdLibary/build/build_windows_speex.sh \
    ThirdLibary/build/build_windows_speexdsp.sh \
    ThirdLibary/build/build_android_speexdsp.sh \
    ThirdLibary/build/build_windows_x264.sh

ANDROID_EXTRA_LIBS = 
