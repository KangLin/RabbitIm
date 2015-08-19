#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

RABBITIM_USE_QXMPP=1         #使用 qxmpp 库
QXMPP_USE_VPX=1              #使用 vpx
#QXMPP_USE_SPEEX=1           #使用 speex
#RABBITIM_USE_OPENCV=1       #使用 opencv
RABBITIM_USE_FFMPEG=1       #使用 ffmpeg
RABBITIM_USE_LIBCURL=1      #使用 libcurl
RABBITIM_USE_OPENSSL=1      #使用openssl
#RABBITIM_USE_STATIC=1       #使用静态编译
#RABBITIM_USE_DOXYGEN=1      #使用doxygen产生文档  
#RABBITIM_USE_PJSIP=1        #使用 pjsip 库
#RABBITIM_USE_PJSIP_CAMERA=1

# 注意：Qt 版本必须大于 5.0  
QT += core gui network xml multimedia widgets 

qtHaveModule(webkit){
    QT += webkitwidgets
    DEFINES += RABBITIM_WEBKIT
}

lessThan(QT_VERSION, 5.0) : error("version is $$QT_VERSION, please qt is used greater then 5.0")

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

CONFIG += c++0x
!msvc{
    QMAKE_CXXFLAGS += " -std=c++0x "
}
CONFIG += link_pkgconfig
#安装
isEmpty(PREFIX){
    android{
       PREFIX=/.

    }
    else{
        PREFIX = $$OUT_PWD/$${TARGET}
    }
}

#equals(RABBITIM_LIBRARY_TYPE, shared) {
#    CONFIG += shared    #生成动态库
#} else {
#    CONFIG += staticlib #生成静态库
#}

equals(RABBITIM_USE_QXMPP, 1) {
    #连接静态QXMPP库时，必须加上-DQXMPP_STATIC。
    #生成静态QXMPP库时，qmake 需要加上 QXMPP_LIBRARY_TYPE=staticlib 参数
    #packagesExist(qxmpp) {
        DEFINES += RABBITIM_USE_QXMPP
        equals(RABBITIM_USE_STATIC, 1){
           DEFINES += QXMPP_STATIC
        }
        CONFIG(release, debug|release) {
            QXMPP_LIBRARY_NAME = -lqxmpp # qxmpp 库名
        }else{
            QXMPP_LIBRARY_NAME = -lqxmpp_d # qxmpp 库名
        }
    #}
}

CONFIG(debug, debug|release) {
    #调试宏
    DEFINES += DEBUG #DEBUG_VIDEO_TIME 
} 

equals(QXMPP_USE_SPEEX, 1) {
    CODEC_LIBRARY += -lspeex
}

equals(QXMPP_USE_VPX, 1) {
    CODEC_LIBRARY += -lvpx
    android {
        CODEC_LIBRARY += -lcpu-features
    }
}

equals(RABBITIM_USE_OPENSSL, 1){
    DEFINES += RABBITIM_USE_OPENSSL
}else{
    RABBITIM_USE_LIBCURL=0
}

equals(RABBITIM_USE_LIBCURL, 1){
    DEFINES += RABBITIM_USE_LIBCURL
    equals(RABBITIM_USE_STATIC, 1) {
        DEFINES += CURL_STATICLIB #用静态库时需要加这个，可以用 ./curl-config --cflags 得到
    }
    DEFINES += CURL_STATICLIB#用静态库时需要加这个，可以用 ./curl-config --cflags 得到
    LIBCURL_LIBRARY = -lcurl #可以用 ./curl-config --libs 得到
}

equals(RABBITIM_USE_FFMPEG, 1) {
    DEFINES += RABBITIM_USE_FFMPEG
    FFMPEG_LIBRARY= -lavcodec -lavformat -lswscale -lavutil
}

equals(RABBITIM_USE_PJSIP, 1){
    DEFINES += RABBITIM_USE_PJSIP

    equals(RABBITIM_USE_PJSIP_CAMERA, 1) {
        DEFINES += RABBITIM_USE_PJSIP_CAMERA
    }

    #PKGCONFIG += libpjproject
    mingw{
    QMAKE_CXXFLAGS+=" -DPJ_AUTOCONF=1 -O2 -DPJ_IS_BIG_ENDIAN=0 -DPJ_IS_LITTLE_ENDIAN=1 "
    PJSIP_LIBRARY=-LD:\msys32\mingw32\lib -lpjsua2-i686-pc-mingw32 \
        -lstdc++ -lpjsua-i686-pc-mingw32 -lpjsip-ua-i686-pc-mingw32 \
        -lpjsip-simple-i686-pc-mingw32 -lpjsip-i686-pc-mingw32 \
        -lpjmedia-codec-i686-pc-mingw32 -lpjmedia-i686-pc-mingw32 \
        -lpjmedia-videodev-i686-pc-mingw32 -lpjmedia-audiodev-i686-pc-mingw32 \
        -lpjmedia-i686-pc-mingw32 -lpjnath-i686-pc-mingw32 -lpjlib-util-i686-pc-mingw32 \
        -lsrtp-i686-pc-mingw32 -lresample-i686-pc-mingw32 -lgsmcodec-i686-pc-mingw32 \
        -lspeex-i686-pc-mingw32 -lilbccodec-i686-pc-mingw32 -lg7221codec-i686-pc-mingw32 \
        -lportaudio-i686-pc-mingw32 -lpj-i686-pc-mingw32 -lssl -lcrypto -lyuv -lm -lwinmm \
        -lole32 -lws2_32 -lwsock32 -lpthread  \
        -L/d/source/rabbitim/ThirdLibary/build_script/../windows_mingw/lib \
        -lavformat -lavicap32 -lgdi32 -lpsapi -lole32 -lstrmiids -luuid -loleaut32 \
        -lws2_32 -liconv -lx264 -lpthread -lm -llzma -lbz2 -lz -lpsapi -ladvapi32 \
        -lshell32 -lavcodec -lavicap32 -lgdi32 -lpsapi -lole32 -lstrmiids -luuid \
        -loleaut32 -lws2_32 -liconv -lx264 -lpthread -lm -llzma -lbz2 -lz -lpsapi \
        -ladvapi32 -lshell32 -lswresample -lm -lswscale -lm -lavutil -lm 
    }else:msvc{
        INCLUDEPATH+="D:\source\pjsip\pjmedia\include" "D:\source\pjsip\pjlib\include" "D:\source\pjsip\include" "D:\source\pjsip/pjnath/include" "D:\source\pjsip/pjlib-util/include"
        PJSIP_LIBRARY=D:\source\pjsip\lib\libpjproject-i386-Win32-vc8-Debug.lib "winmm.lib" "dsound.lib" "dxguid.lib" "netapi32.lib" "mswsock.lib" "ws2_32.lib" "odbc32.lib" "odbccp32.lib" "ole32.lib" "user32.lib" "kernel32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "oleaut32.lib" "uuid.lib" 
    }else:android{
        QMAKE_CXXFLAGS +=" -DPJ_AUTOCONF=1 -fpic -ffunction-sections -funwind-tables -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -Os -g -DNDEBUG -fomit-frame-pointer -finline-limit=64 -O0 -UNDEBUG -marm -fno-omit-frame-pointer -Ijni -DANDROID -Wa,--noexecstack -Wformat -Werror=format-security -ID:/software/android-ndk-r9/platforms/android-18/arch-arm/usr/include  -I/d/software/android-ndk-r9/sources/cxx-stl/gnu-libstdc++/4.8/include -I/d/software/android-ndk-r9/sources/cxx-stl/gnu-libstdc++/4.8/libs/armeabi-v7a/include -DPJ_IS_BIG_ENDIAN=0 -DPJ_IS_LITTLE_ENDIAN=1"
        PJSIP_LIBRARY=-lpjsua2-arm-unknown-linux-androideabi -lstdc++ \
            -lpjsua-arm-unknown-linux-androideabi -lpjsip-ua-arm-unknown-linux-androideabi \
            -lpjsip-simple-arm-unknown-linux-androideabi -lpjsip-arm-unknown-linux-androideabi \
            -lpjmedia-codec-arm-unknown-linux-androideabi -lpjmedia-arm-unknown-linux-androideabi \
            -lpjmedia-videodev-arm-unknown-linux-androideabi -lpjmedia-audiodev-arm-unknown-linux-androideabi \
            -lpjmedia-arm-unknown-linux-androideabi -lpjnath-arm-unknown-linux-androideabi \
            -lpjlib-util-arm-unknown-linux-androideabi  -lsrtp-arm-unknown-linux-androideabi \
            -lresample-arm-unknown-linux-androideabi -lgsmcodec-arm-unknown-linux-androideabi \
            -lspeex-arm-unknown-linux-androideabi -lilbccodec-arm-unknown-linux-androideabi \
            -lg7221codec-arm-unknown-linux-androideabi  -lpj-arm-unknown-linux-androideabi -lm \
            -lgnustl_static  -lc -lgcc -ldl -lOpenSLES -llog -lGLESv2 -lEGL -landroid
    }
}

#android选项中包含了unix选项，所以在写工程如下条件判断时，必须把android条件放在unix条件前
android{
    THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/android
    DEFINES += ANDROID MOBILE

    RABBITIM_SYSTEM="android"
    equals(RABBITIM_USE_LIBCURL, 1){
        LIBCURL_LIBRARY = -lcurl -lssl -lcrypto -lz #可以用 ./curl-config --libs 得到  
    }
    equals(RABBITIM_USE_OPENSSL, 1){
        LIBOPENSSL_LIBRARY = -lssl -lcrypto
    }
}else:win32{
    RABBITIM_SYSTEM="windows"
    OPENCV_VERSION=300
    msvc {
        RABBITIM_PLATFORM="msvc"
        QMAKE_CXXFLAGS += /wd"4819"  #忽略msvc下对utf-8的警告  
        LDFLAGS += -ladvapi32
        THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/windows_msvc

        equals(RABBITIM_USE_LIBCURL, 1){
            equals(RABBITIM_USE_STATIC, 1){
                LIBCURL_LIBRARY = -llibcurl_a
            }else{
                LIBCURL_LIBRARY = -llibcurl 
            }
        }
        equals(RABBITIM_USE_OPENSSL, 1){
            LIBOPENSSL_LIBRARY = -lssleay32 -llibeay32
        }
    }
    else {
        RABBITIM_PLATFORM="mingw"
        THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/windows_mingw

        equals(RABBITIM_USE_LIBCURL, 1){
            LIBCURL_LIBRARY = -lcurl -lssl -lcrypto -lgdi32 -lcrypt32 -lwldap32 -lz -lws2_32 #可以用 ./curl-config --libs 得到
         }
        equals(RABBITIM_USE_OPENSSL, 1){
            LIBOPENSSL_LIBRARY = -lssl -lcrypto
        }
    }

    CONFIG(release, debug|release){
        msvc{
            LDFLAGS += /NODEFAULTLIB:libcmt /SUBSYSTEM:WINDOWS",5.01"
        }

        equals(RABBITIM_USE_STATIC, 1) {
            QXMPP_LIBRARY_NAME = -lqxmpp # qxmpp 库名
        }else{
            QXMPP_LIBRARY_NAME = -lqxmpp0 # qxmpp 库名
        }
    } else:CONFIG(debug, debug|release) {
        msvc{
            LDFLAGS += /NODEFAULTLIB:libcmtd /SUBSYSTEM:WINDOWS",5.01"
        }

        equals(RABBITIM_USE_STATIC, 1) {
            QXMPP_LIBRARY_NAME = -lqxmpp_d # qxmpp 库名
        }else{
            QXMPP_LIBRARY_NAME = -lqxmpp_d0 # qxmpp 库名
        }
    }

    WEBRTC_LIBRARY_DIR = .
    #WEBRTC_LIBRARY = -L$$WEBRTC_LIBRARY_DIR -llibjingle -llibjingle_media -llibjingle_p2p -lwebrtc
}else:unix{
    RABBITIM_SYSTEM=unix
    DEFINES += UNIX
    THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/unix

    equals(RABBITIM_USE_LIBCURL, 1){
        LIBCURL_LIBRARY = -lcurl -lssl -lcrypto -lz  #可以用 ./curl-config --libs 得到
    }
    equals(RABBITIM_USE_OPENSSL, 1){
        LIBOPENSSL_LIBRARY = -lssl -lcrypto
    }
}
equals(RABBITIM_USE_STATIC, 1):exists("$${THIRD_LIBRARY_PATH}_static"){
    THIRD_LIBRARY_PATH=$${THIRD_LIBRARY_PATH}_static
    message("THIRD_LIBRARY_PATH:$${THIRD_LIBRARY_PATH}")
}
INCLUDEPATH += $$PWD $$PWD/Widgets/FrmCustom
INCLUDEPATH += $${THIRD_LIBRARY_PATH}/include $$WEBRTC_ROOT
DEPENDPATH += $${THIRD_LIBRARY_PATH}/include $$WEBRTC_ROOT
LIBS += -L$${THIRD_LIBRARY_PATH}/lib  

equals(RABBITIM_USE_OPENCV, 1) {
    DEFINES += RABBITIM_USE_OPENCV
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
}else:android{
    message("android video capture need opencv, please set RABBITIM_USE_OPENCV=1")
}

LIBS += $$LDFLAGS $$PJSIP_LIBRARY $$QXMPP_LIBRARY_NAME $$WEBRTC_LIBRARY \
        $$OPENCV_LIBRARY $$FFMPEG_LIBRARY $$CODEC_LIBRARY \
        $$LIBCURL_LIBRARY $$LIBOPENSSL_LIBRARY 
message("Libs:$$LIBS")

DEFINES += __STDC_CONSTANT_MACROS #ffmpeg需要

include(RabbitIm.pri)
include(Lbs/Lbs.pri)
#发行版本才更新更新配置
CONFIG(release, debug|release) {
    include(RabbitIm.prf)
    !equals(RABBITIM_USE_LIBCURL, 1){
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
    .travis.yml \
    debian/* \
    ThirdLibary/build_script/travis/*.sh \
    ThirdLibary/build_script/*.sh \
    AppIcon.rc \
    Resource/translations/app_zh_CN.ts \
    docs/* \
    docs/Books/* \
    docs/QXmpp音视频呼叫流程.txt \
    License.md \
    ChangeLog.md \
    Authors.txt \
    CMakeLists.txt \
    cmake/* \
    cmake/platforms/* \
    platforms/android/* \
    Update/*template* \
    Update/*.xml \
    Doxyfile*

# Rules for creating/updating {ts|qm}-files
include(Resource/translations/translations.pri)
# Build all the qm files now, to make RCC happy
#system($$fromfile(Resource/translations/translations.pri, updateallqm))

#ANDROID 平台相关内容  
android{
    include(android/android.pri)
}

contains(ANDROID_TARGET_ARCH,armeabi-v7a){
    ANDROID_PERMISSIONS += \
        android.permission.CAMERA

    ANDROID_FEATURES += \
        android.hardware.camera
    equals(RABBITIM_USE_OPENCV, 1){
        ANDROID_EXTRA_LIBS = \
            $$PWD/ThirdLibary/android/lib/libnative_camera_r4.2.0.so  \ #TODO:修改成你手机平台对应的版本，如果没有，则取最近的版本
            $$PWD/ThirdLibary/android/lib/libopencv_info.so #\
            #$$PWD/ThirdLibary/android/lib/libopencv_core.so \
            #$$PWD/ThirdLibary/android/lib/libopencv_imgproc.so \
            #$$PWD/ThirdLibary/android/lib/libopencv_videoio.so \
            #$$PWD/ThirdLibary/android/lib/libopencv_flann.so \
            #$$PWD/ThirdLibary/android/lib/libopencv_imgcodecs.so \
            #$$PWD/ThirdLibary/android/lib/libopencv_video.so
            #$$PWD/ThirdLibary/android/lib/libnative_camera_r2.2.0.so \
            #$$PWD/ThirdLibary/android/lib/libnative_camera_r4.1.1.so \
            #$$PWD/ThirdLibary/android/lib/libnative_camera_r2.3.3.so \
            #$$PWD/ThirdLibary/android/lib/libnative_camera_r3.0.1.so \
            #$$PWD/ThirdLibary/android/lib/libnative_camera_r4.3.0.so \
            #$$PWD/ThirdLibary/android/lib/libnative_camera_r4.0.0.so \
            #$$PWD/ThirdLibary/android/lib/libnative_camera_r4.4.0.so \
            #$$PWD/ThirdLibary/android/lib/libnative_camera_r4.0.3.so \
    }
}

win32{
    #安装qt依赖库
    Deployment_qtlib.target = Deployment_qtlib
    Deployment_qtlib.path = $${PREFIX}
    Deployment_qtlib.commands = "$$[QT_INSTALL_BINS]/windeployqt" \
                    --compiler-runtime \
                    --verbose 7 \
                    "$${PREFIX}/$${TARGET}.exe"
    #安装第三方依赖库
    Deployment_third_lib.target = Deployment_third_lib
    Deployment_third_lib.files = $${THIRD_LIBRARY_PATH}/bin/*.dll
    Deployment_third_lib.path = $$PREFIX
    Deployment_third_lib.CONFIG += directory no_check_exist

    INSTALLS += Deployment_qtlib Deployment_third_lib

    #复制第三方依赖库动态库到编译输出目录 
    THIRD_LIBRARY_DLL = $${THIRD_LIBRARY_PATH}/bin/*.dll
    equals(QMAKE_HOST.os, Windows):isEmpty(QMAKE_SH){
        THIRD_LIBRARY_DLL =  $$replace(THIRD_LIBRARY_DLL, /, \\)
        TARGET_PATH = $$replace(TARGET_PATH, /, \\)
        TARGET_PATH = $${TARGET_PATH}\.
    }else{
        TARGET_PATH = $${TARGET_PATH}/.
    }

    exists($${THIRD_LIBRARY_DLL}){
        ThirdLibraryDll.commands =  \
            $${QMAKE_COPY} $$THIRD_LIBRARY_DLL $${TARGET_PATH}
        ThirdLibraryDll.CONFIG += directory no_link no_clean no_check_exist
        ThirdLibraryDll.target = ThirdLibraryDll
        QMAKE_EXTRA_TARGETS += ThirdLibraryDll
        POST_TARGETDEPS += ThirdLibraryDll
    }
}
