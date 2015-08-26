#-------------------------------------------------
#
# Project created by QtCreator 2015-08-02T13:43:29
#
#-------------------------------------------------

#RABBITIM_USE_LIBCURL=1      #使用 libcurl
#RABBITIM_USE_OPENSSL=1      #使用openssl

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++0x
!msvc{
    QMAKE_CXXFLAGS += " -std=c++0x "
}

TARGET = lbs
TEMPLATE = app

CONFIG += mobility
MOBILITY = 

CONFIG(debug, debug|release) {
    #调试宏
    DEFINES += DEBUG #DEBUG_VIDEO_TIME 
} 

equals(RABBITIM_USE_OPENSSL, 1){
    DEFINES += RABBITIM_USE_OPENSSL
}else{
    RABBITIM_USE_LIBCURL=0
}

equals(RABBITIM_USE_LIBCURL, 1){
    DEFINES += RABBITIM_USE_LIBCURL
    equals(RABBITIM_USE_STATIC, 1) {
        CURL_STATICLIB#用静态库时需要加这个，可以用 ./curl-config --cflags 得到
    }
    LIBCURL_LIBRARY = -lcurl #可以用 ./curl-config --libs 得到
}

#android选项中包含了unix选项，所以在写工程如下条件判断时，必须把android条件放在unix条件前
android{
    THIRD_LIBRARY_PATH = $$PWD/../ThirdLibary/android
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
        THIRD_LIBRARY_PATH = $$PWD/../ThirdLibary/windows_msvc

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
        THIRD_LIBRARY_PATH = $$PWD/../ThirdLibary/windows_mingw

        equals(RABBITIM_USE_LIBCURL, 1){
            LIBCURL_LIBRARY = -lcurl -lssl -lcrypto -lgdi32 -lwldap32 -lws2_32 #可以用 ./curl-config --libs 得到
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
    THIRD_LIBRARY_PATH = $$PWD/../ThirdLibary/unix

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
INCLUDEPATH += $$PWD $$PWD/../.. $$PWD/Widgets/FrmCustom
INCLUDEPATH += $${THIRD_LIBRARY_PATH}/include $$WEBRTC_ROOT
DEPENDPATH += $${THIRD_LIBRARY_PATH}/include $$WEBRTC_ROOT
LIBS += -L$${THIRD_LIBRARY_PATH}/lib  

LIBS += $$LIBCURL_LIBRARY $$LIBOPENSSL_LIBRARY 
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
#安装
isEmpty(PREFIX){
    android{
       PREFIX=/.

    }
    else{
        PREFIX = $$OUT_PWD/$${TARGET}
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

    target.file = $${TARGET}
    target.path = $$PREFIX
    INSTALLS += target Deployment_qtlib Deployment_third_lib

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

include(Lbs.pri)
SOURCES += main.cpp \
    ../../Global/Log.cpp \
    ../../Plugin/PluginApp.cpp

HEADERS  += \
    ../../Global/Log.h \
    ../../Plugin/PluginApp.h

RESOURCES += \
    ../../Resource/Resource.qrc \
    ../../Resource/sink/dark/style.qrc
