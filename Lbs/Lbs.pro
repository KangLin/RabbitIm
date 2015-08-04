#-------------------------------------------------
#
# Project created by QtCreator 2015-08-02T13:43:29
#
#-------------------------------------------------

RABBITIM_USE_LIBCURL=1      #使用 libcurl
RABBITIM_USE_OPENSSL=1      #使用openssl

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
 QT += positioning

TARGET = lbs
TEMPLATE = app

CONFIG += mobility
MOBILITY = 

include(Lbs.pri)
SOURCES += main.cpp


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
INCLUDEPATH += $$PWD $$PWD/Widgets/FrmCustom
INCLUDEPATH += $${THIRD_LIBRARY_PATH}/include $$WEBRTC_ROOT
DEPENDPATH += $${THIRD_LIBRARY_PATH}/include $$WEBRTC_ROOT
LIBS += -L$${THIRD_LIBRARY_PATH}/lib  


LIBS += $$LIBCURL_LIBRARY $$LIBOPENSSL_LIBRARY 
