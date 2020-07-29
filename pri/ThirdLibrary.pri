#第三方依赖库  

RABBITIM_USE_QXMPP=1         #使用 qxmpp 库  
QXMPP_USE_VPX=1              #使用 vpx
#QXMPP_USE_SPEEX=1           #使用 speex  
QXMPP_USE_OPUS=1             #使用 opus
#RABBITIM_USE_OPENCV=1       #使用 opencv  
RABBITIM_USE_FFMPEG=1       #使用 ffmpeg
#RABBITIM_USE_LIBCURL=1      #使用 libcurl  
#RABBITIM_USE_OPENSSL=1      #使用openssl
#RABBITIM_USE_DOXYGEN=1      #使用doxygen产生文档  
#RABBITIM_USE_PJSIP=1        #使用 pjsip 库  
#RABBITIM_USE_PJSIP_CAMERA=1  
#RABBITIM_USE_QRENCODE=1
RABBITIM_USE_QZXING=1

equals(RABBITIM_USE_QXMPP, 1) {
    DEFINES *= RABBITIM_USE_QXMPP

    myPackagesExist(qxmpp) {
        MYPKGCONFIG *= qxmpp
    }

    msvc {
        LIBS *= -lqxmpp
        INCLUDEPATH *= $${THIRD_LIBRARY_PATH}/include/qxmpp
    }
}

equals(RABBITIM_USE_FFMPEG, 1) {
    DEFINES *= __STDC_CONSTANT_MACROS #ffmpeg需要  
    myPackagesExist(libavcodec libavformat libswscale libavutil) {
        MYPKGCONFIG *= libavcodec libavformat libswscale libavutil 
        DEFINES *= RABBITIM_USE_FFMPEG
    } else : msvc {
        LIBS += -lavcodec -lavformat -lswscale -lavutil
        DEFINES *= RABBITIM_USE_FFMPEG
    }
    myPackagesExist(libswresample){
        MYPKGCONFIG *= libswresample
    } else : msvc {
        LIBS += -lswresample
    }

    myPackagesExist(x264) {
        MYPKGCONFIG *= x264
    }
}

equals(RABBITIM_USE_PJSIP, 1) : myPackagesExist(libpjproject){
    DEFINES += RABBITIM_USE_PJSIP
    equals(RABBITIM_USE_PJSIP_CAMERA, 1) {
        DEFINES += RABBITIM_USE_PJSIP_CAMERA
    }
    MYPKGCONFIG *= libpjproject
}

equals(RABBITIM_USE_OPENCV, 1){
    DEFINES *= RABBITIM_USE_OPENCV
    OPENCV_LIBRARY= -lopencv_video$$OPENCV_VERSION -lopencv_videoio$$OPENCV_VERSION \
                    -lopencv_imgproc$$OPENCV_VERSION \
                    -lopencv_core$$OPENCV_VERSION

    android{
        OPENCV_LIBRARY += \
                         -lopencv_androidcamera \
                         -lopencv_imgcodecs \
                         -lopencv_info \
                         -llibjpeg
    }else{
        OPENCV_LIBRARY += -lzlib
    }

    win32{
        OPENCV_LIBRARY += -lopencv_imgcodecs$$OPENCV_VERSION
        OPENCV_LIBRARY += -lOle32 -lolepro32 -loleaut32 -luuid #dshow依赖库  
    }
}else:android{
    message("android video capture need opencv, please set RABBITIM_USE_OPENCV=1")
}

equals(QXMPP_USE_SPEEX, 1) : myPackagesExist(speex) {
    MYPKGCONFIG *= speex
}

equals(QXMPP_USE_OPUS, 1) {
    myPackagesExist(opus) {
        MYPKGCONFIG *= opus
    }

    msvc {
        LIBS *= -lopus
        INCLUDEPATH *= $${THIRD_LIBRARY_PATH}/include/opus
    }
}

equals(QXMPP_USE_VPX, 1) {
    myPackagesExist(vpx){
        MYPKGCONFIG *= vpx
    }
    msvc {
        LIBS *= -lvpx
        INCLUDEPATH *= $${THIRD_LIBRARY_PATH}/include/vpx
    }
}

equals(RABBITIM_USE_QRENCODE, 1){
    msvc {
        DEFINES *= RABBITIM_USE_LIBQRENCODE
        CONFIG(debug, debug|release) {
            LIBS *= -lqrencoded
        } 
        CONFIG(release, debug|release) {
            LIBS += -lqrencode
        }
    } else {
        myPackagesExist(libqrencode) {
            DEFINES *= RABBITIM_USE_LIBQRENCODE
            MYPKGCONFIG *= libqrencode
        } 
    }
}

equals(RABBITIM_USE_QZXING, 1) {
    myPackagesExist(QZXing) {
        DEFINES *= RABBITIM_USE_QZXING ENABLE_ENCODER_GENERIC
        MYPKGCONFIG *= QZXing
    }

    msvc {
        DEFINES *= RABBITIM_USE_QZXING ENABLE_ENCODER_GENERIC
        LIBS *= -lQZXing2
    }
}

include(Webrtc.pri)

equals(RABBITIM_USE_LIBCURL, 1) {
    myPackagesExist(libcurl) {
        DEFINES *= RABBITIM_USE_LIBCURL
        MYPKGCONFIG *= libcurl
    } else : msvc {
        DEFINES *= RABBITIM_USE_LIBCURL
        CONFIG(debug, debug|release){
            LIBS += -llibcurl-d_imp
        } else {
            LIBS += -llibcurl_imp
        }
    }
}

equals(RABBITIM_USE_OPENSSL, 1) {
    myPackagesExist(openssl) {
        DEFINES *= RABBITIM_USE_OPENSSL
        MYPKGCONFIG *= openssl
    } else : msvc {
        DEFINES *= RABBITIM_USE_OPENSSL
        LIBS += -llibssl -llibcrypto #-llibeay32 -lssleay32 
    }
}
