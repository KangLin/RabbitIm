#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

QXMPP_USE_VPX = 1             #使用 vpx
#QXMPP_USE_SPEEX=1            #使用 speex
#RABBITIM_USER_OPENCV=1       #使用 opencv
RABBITIM_USER_FFMPEG=1       #使用 ffmpeg

# 注意：Qt 版本必须大于 5.0  
QT += core gui network xml multimedia widgets

lessThan(QT_MAJOR_VERSION, 5) : error("version is $$QT_MAJOR_VERSION, please qt is used greater then 5.0")

TARGET = RabbitIm
TEMPLATE = app
#设置目标输出目录
win32{
    CONFIG(debug, debug|release){
        TARGET_PATH=$${OUT_PWD}/Debug
    }else{
        TARGET_PATH=$${OUT_PWD}/Release
    }
}else{
    TARGET_PATH=$${OUT_PWD}
}


CONFIG   += c++11

#预编译
#CONFIG += precompiled_header
#PRECOMPILED_HEADER = pch.h

#equals(RABBITIM_LIBRARY_TYPE, shared) {
#    CONFIG += shared    #生成动态库
#} else {
#    CONFIG += staticlib #生成静态库
#}

#连接静态QXMPP库时，必须加上-DQXMPP_STATIC。生成静态QXMPP库时，qmake 需要加上 QXMPP_LIBRARY_TYPE=staticlib 参数
DEFINES += QXMPP QXMPP_STATIC
QXMPP_LIBRARY_NAME = -lqxmpp# qxmpp 库名

CONFIG(debug, debug|release) {
    #调试宏
    DEFINES += DEBUG DEBUG_VIDEO_TIME 
} 

!isEmpty(QXMPP_USE_SPEEX) {
    CODEC_LIBRARY += -lspeex
}

!isEmpty(QXMPP_USE_VPX) {
    CODEC_LIBRARY += -lvpx
    android {
        CODEC_LIBRARY += -lcpu-features
    }
}

!isEmpty(RABBITIM_USER_FFMPEG) {
    DEFINES += RABBITIM_USER_FFMPEG
    FFMPEG_LIBRARY= -lavcodec -lavformat -lswscale  -lavfilter  -lavutil
}
INCLUDEPATH += $$PWD $$PWD/Widgets/FrmCustom 

#android选项中包含了unix选项，所以在写工程如下条件判断时，必须把android条件放在unix条件前
android{
    INCLUDEPATH += $$PWD/ThirdLibary/android/include $$WEBRTC_ROOT
    DEPENDPATH += $$PWD/ThirdLibary/android/include $$WEBRTC_ROOT
    DEFINES += ANDROID MOBILE
    LIBS += -L$$PWD/ThirdLibary/android/lib  
} else:win32{
    msvc {
        LDFLAGS += -ladvapi32

        INCLUDEPATH += $$PWD/ThirdLibary/windows_msvc/include $$WEBRTC_ROOT
        DEPENDPATH += $$PWD/ThirdLibary/windows_msvc/include $$WEBRTC_ROOT
        LIBS += -L$$PWD/ThirdLibary/windows_msvc/lib

        QXMPP_LIBRARY_NAME = qxmpp.lib# qxmpp 库名

        !isEmpty(RABBITIM_USER_FFMPEG) {
            #msvc 下直接用库文名查找依赖库
            FFMPEG_LIBRARY= libavcodec.a libavformat.a libswscale.a libswresample.a libavfilter.a libavutil.a
        }
    }
    else
    {
        INCLUDEPATH += $$PWD/ThirdLibary/windows_mingw/include $$WEBRTC_ROOT
        DEPENDPATH += $$PWD/ThirdLibary/windows_mingw/include $$WEBRTC_ROOT
        LIBS += -L$$PWD/ThirdLibary/windows_mingw/lib
    }

    CONFIG(release, debug|release){
        msvc{
            LDFLAGS += /NODEFAULTLIB:libcmt
        }

    } else:CONFIG(debug, debug|release){
        DEFINES += DEBUG

        msvc{
            LDFLAGS += /NODEFAULTLIB:libcmtd /NODEFAULTLIB:libcmt
            QXMPP_LIBRARY_NAME = qxmpp_d.lib# qxmpp 库名
        }

        OPENCV_VERSION=300
    }

    WEBRTC_LIBRARY_DIR = .
    #WEBRTC_LIBRARY = -L$$WEBRTC_LIBRARY_DIR -llibjingle -llibjingle_media -llibjingle_p2p -lwebrtc
} else:unix {
    DEFINES += UNIX
    INCLUDEPATH += $$PWD/ThirdLibary/unix/include $$WEBRTC_ROOT
    DEPENDPATH += $$PWD/ThirdLibary/unix/include $$WEBRTC_ROOT

    LIBS += -L$$PWD/ThirdLibary/unix/lib
}

!isEmpty(RABBITIM_USER_OPENCV) {
    DEFINES += RABBITIM_USER_OPENCV
    OPENCV_LIBRARY= -lopencv_video$$OPENCV_VERSION \
                                            -lopencv_videoio$$OPENCV_VERSION \
                                            -lopencv_imgproc$$OPENCV_VERSION \
                                            -lopencv_core$$OPENCV_VERSION

    android{
        OPENCV_LIBRARY += \
                                              -lopencv_androidcamera \
                                              -lopencv_imgcodecs \
                                              -lopencv_info \
                                              -llibjpeg
    }
    else {
        OPENCV_LIBRARY += -lzlib
    }

    win32{
        OPENCV_LIBRARY += -lopencv_imgcodecs$$OPENCV_VERSION 
        OPENCV_LIBRARY += -lOle32 -lolepro32 -loleaut32 -luuid #dshow依赖库
    }
}

LIBS += $$LDFLAGS $$QXMPP_LIBRARY_NAME $$WEBRTC_LIBRARY $$OPENCV_LIBRARY $$FFMPEG_LIBRARY $$CODEC_LIBRARY

DEFINES += __STDC_CONSTANT_MACROS #ffmpeg需要

include(RabbitIm.pri)

CONFIG += mobility

MOBILITY = 

#应用程序图标
RC_FILE = AppIcon.rc

CONFIG += localize_deployment  #本地语言部署

OTHER_FILES += README.md \
    .gitignore \
    AppIcon.rc \
    Resource/translations/app_zh_CN.ts \
    docs/* \
    docs/Books/* \
    docs/QXmpp音视频呼叫流程.txt \
    ThirdLibary/build_script/*.sh \
    android/AndroidManifest.xml \
    License.html \
    ChangeLog.txt \
    Authors.txt \
    CMakeLists.txt \
    cmake/* \
    platforms/android/*

# Rules for creating/updating {ts|qm}-files
include(Resource/translations/translations.pri)
# Build all the qm files now, to make RCC happy
#system($$fromfile(Resource/translations/translations.pri, updateallqm))

#ANDROID 平台相关内容  
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android  #ANDROID包的源码目录

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PERMISSIONS += \
        android.permission.CAMERA

    ANDROID_FEATURES += \
        android.hardware.camera
    !isEmpty(RABBITIM_USER_OPENCV) {
        ANDROID_EXTRA_LIBS = \
            $$PWD/ThirdLibary/android/lib/libnative_camera_r4.0.3.so \
            $$PWD/ThirdLibary/android/lib/libopencv_info.so 
    }
}
