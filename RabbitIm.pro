#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

QXMPP_USE_VPX = 1             #使用 vpx
#QXMPP_USE_SPEEX=1            #使用 speex
#RABBITIM_USER_OPENCV=1       #使用 opencv
RABBITIM_USER_FFMPEG=1       #使用 ffmpeg
#RABBITIM_USER_LIBCURL=1      #使用 libcurl
#RABBITIM_USER_OPENSSL=1   #使用openssl

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

CONFIG += c++11

#安装
isEmpty(PREFIX){
    android{
       PREFIX=/.
    }
    else{
        PREFIX = $$PWD/install
    }
}

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

!isEmpty(RABBITIM_USER_OPENSSL){
    DEFINES+= RABBITIM_USER_OPENSSL
}

!isEmpty(RABBITIM_USER_LIBCURL){
    DEFINES += RABBITIM_USER_LIBCURL CURL_STATICLIB#用静态库时需要加这个，可以用 ./curl-config --cflags 得到
    LIBCURL_LIBRARY = -lcurl #可以用 ./curl-config --libs 得到
}

!isEmpty(RABBITIM_USER_FFMPEG) {
    DEFINES += RABBITIM_USER_FFMPEG
    FFMPEG_LIBRARY= -lavcodec -lavformat -lswscale -lavfilter  -lavutil
}
INCLUDEPATH += $$PWD $$PWD/Widgets/FrmCustom 

#android选项中包含了unix选项，所以在写工程如下条件判断时，必须把android条件放在unix条件前
android{
    THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/android
    DEFINES += ANDROID MOBILE
    
    RABBITIM_SYSTEM="android"
    !isEmpty(RABBITIM_USER_LIBCURL){
        LIBCURL_LIBRARY = -lcurl -lssl -lcrypto -lz#可以用 ./curl-config --libs 得到
    }else:!isEmpty(RABBITIM_USER_OPENSSL){
        LIBOPENSSL_LIBRARY = -lssl -lcrypto
    }
} else:win32{
    RABBITIM_SYSTEM="windows"
    msvc {
        RABBITIM_PLATFORM="msvc"

        LDFLAGS += -ladvapi32
        THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/windows_msvc
        QXMPP_LIBRARY_NAME = qxmpp.lib# qxmpp 库名

        !isEmpty(RABBITIM_USER_FFMPEG) {
            #msvc 下直接用库文名查找依赖库
            FFMPEG_LIBRARY= libavcodec.a libavformat.a libswscale.a libswresample.a libavfilter.a libavutil.a
        }

        !isEmpty(RABBITIM_USER_LIBCURL){
            LIBCURL_LIBRARY = -lcurl 
        }else:!isEmpty(RABBITIM_USER_OPENSSL){
            LIBOPENSSL_LIBRARY = ssleay32.lib libeay32.lib
        }
    }
    else {
        RABBITIM_PLATFORM="mingw"
        THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/windows_mingw

        !isEmpty(RABBITIM_USER_LIBCURL){
            LIBCURL_LIBRARY = -lcurl -lssl -lcrypto -lgdi32 -lwldap32 -lz -lws2_32#可以用 ./curl-config --libs 得到
        }else{
            !isEmpty(RABBITIM_USER_OPENSSL){
                LIBOPENSSL_LIBRARY = -lssl -lcrypto
            }
        }
    }

    CONFIG(release, debug|release){
        msvc{
            LDFLAGS += /NODEFAULTLIB:libcmt
        }

    } else:CONFIG(debug, debug|release){
        msvc{
            LDFLAGS += /NODEFAULTLIB:libcmtd /NODEFAULTLIB:libcmt
            QXMPP_LIBRARY_NAME = qxmpp_d.lib# qxmpp 库名
        }

        OPENCV_VERSION=300
    }

    WEBRTC_LIBRARY_DIR = .
    #WEBRTC_LIBRARY = -L$$WEBRTC_LIBRARY_DIR -llibjingle -llibjingle_media -llibjingle_p2p -lwebrtc
} else:unix {
    RABBITIM_SYSTEM=unix
    DEFINES += UNIX
    THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/unix
}

INCLUDEPATH += $${THIRD_LIBRARY_PATH}/include $$WEBRTC_ROOT
DEPENDPATH += $${THIRD_LIBRARY_PATH}/include $$WEBRTC_ROOT
LIBS += -L$${THIRD_LIBRARY_PATH}/lib  

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

LIBS += $$LDFLAGS $$QXMPP_LIBRARY_NAME $$WEBRTC_LIBRARY \
        $$OPENCV_LIBRARY $$FFMPEG_LIBRARY $$CODEC_LIBRARY \
        $$LIBCURL_LIBRARY $$LIBOPENSSL_LIBRARY

DEFINES += __STDC_CONSTANT_MACROS #ffmpeg需要

include(RabbitIm.pri)
#发行版本才更新更新配置
CONFIG(release, debug|release) {
    include(RabbitIm.prf)
    isEmpty(RABBITIM_USER_LIBCURL){
        warning("don't update function")
    }
}

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
    platforms/android/* \
    Update/Update*.xml* \
    Version.h.template

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
            $$PWD/ThirdLibary/android/lib/libnative_camera_r4.2.0.so  \ #修改成你手机平台对应的版本，如果没有，则取最近的版本
            $$PWD/ThirdLibary/android/lib/libopencv_info.so 
    }
}

win32{
    #安装qt依赖库
    Deployment_qtlib.target = Deployment_qtlib
    Deployment_qtlib.path = $${PREFIX}
    Deployment_qtlib.commands = "$$[QT_INSTALL_BINS]/windeployqt" \
                    --compiler-runtime \
                    --no-translations \
                    --verbose 7 \
                    "$${PREFIX}/$${TARGET}.exe"
    #安装第三方依赖库
    Deployment_third_lib.target = Deployment_third_lib
    Deployment_third_lib.files = $${THIRD_LIBRARY_PATH}/bin/*.dll
    Deployment_third_lib.path = $$PREFIX
    Deployment_third_lib.CONFIG += directory no_check_exist

    INSTALLS += Deployment_qtlib Deployment_third_lib
}

win32 {
    #复制第三方依赖库动态库到编译输出目录 
    THIRD_LIBRARY_DLL =  $${THIRD_LIBRARY_PATH}/bin/*.dll
    THIRD_LIBRARY_DLL =  $$replace(THIRD_LIBRARY_DLL, /, \\)
    exists($${THIRD_LIBRARY_DLL}){
        ThirdLibraryDll.commands =  \
            $(COPY) $$THIRD_LIBRARY_DLL $${TARGET_PATH}\.
        ThirdLibraryDll.CONFIG += directory no_link no_clean no_check_exist
        ThirdLibraryDll.target = ThirdLibraryDll
        QMAKE_EXTRA_TARGETS += ThirdLibraryDll
        POST_TARGETDEPS += ThirdLibraryDll
    }
}
