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

#第三方依赖库  
equals(RABBITIM_USE_QXMPP, 1) {
    CONFIG(release, debug|release) {
        myPackagesExist(qxmpp) {
            DEFINES *= RABBITIM_USE_QXMPP
            MYPKGCONFIG += qxmpp
        } else : msvc {
            DEFINES *= RABBITIM_USE_QXMPP
            LIBS += -lqxmpp0
        }
    } else {
        myPackagesExist(qxmpp_d) {
            DEFINES *= RABBITIM_USE_QXMPP
            MYPKGCONFIG += qxmpp_d
        }else : msvc {
            DEFINES *= RABBITIM_USE_QXMPP
            LIBS += -lqxmpp_d0
        }
    }
}

equals(RABBITIM_USE_FFMPEG, 1) : packagesExist(libavcodec libavformat libswscale libavutil) {
    DEFINES *= RABBITIM_USE_FFMPEG __STDC_CONSTANT_MACROS #ffmpeg需要  
    MYPKGCONFIG *= libavcodec libavformat libswscale libavutil 
}

equals(RABBITIM_USE_PJSIP, 1) : packagesExist(libpjproject){
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

equals(QXMPP_USE_SPEEX, 1) : packagesExist(speex) {
    MYPKGCONFIG *= speex
}

equals(QXMPP_USE_VPX, 1) : packagesExist(vpx){
    MYPKGCONFIG *= vpx
}

packagesExist(libqrencode) {
    DEFINES *= RABBITIM_USE_LIBQRENCODE
    MYPKGCONFIG *= libqrencode
}

packagesExist(QZXing) {
    DEFINES *= RABBITIM_USE_QZXING
    MYPKGCONFIG *= QZXing
} else : msvc {
    DEFINES *= RABBITIM_USE_QZXING
    LIBS += -lQZXing2
}

equals(RABBITIM_USE_LIBCURL, 1) {
    packagesExist(libcurl) {
        DEFINES *= RABBITIM_USE_LIBCURL
        MYPKGCONFIG *= libcurl
    } else : msvc {
        DEFINES *= RABBITIM_USE_LIBCURL
        LIBS += -llibcurl
    }
}

equals(RABBITIM_USE_OPENSSL, 1) {
    packagesExist(openssl) {
        DEFINES *= RABBITIM_USE_OPENSSL
        MYPKGCONFIG *= openssl
    } else : msvc {
        DEFINES *= RABBITIM_USE_OPENSSL
        LIBS += -llibeay32 -lssleay32 
    }
}
