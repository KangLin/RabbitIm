#第三方依赖库  

RABBITIM_USE_QXMPP=1         #使用 qxmpp 库  
QXMPP_USE_VPX=1              #使用 vpx  
#QXMPP_USE_SPEEX=1           #使用 speex  
#RABBITIM_USE_OPENCV=1       #使用 opencv  
RABBITIM_USE_FFMPEG=1       #使用 ffmpeg  
RABBITIM_USE_LIBCURL=1      #使用 libcurl  
RABBITIM_USE_OPENSSL=1      #使用openssl    
#RABBITIM_USE_DOXYGEN=1      #使用doxygen产生文档  
#RABBITIM_USE_PJSIP=1        #使用 pjsip 库  
#RABBITIM_USE_PJSIP_CAMERA=1  
#RABBITIM_USE_WEBRTC=1        #使用 webrtc 库  

equals(RABBITIM_USE_QXMPP, 1) {
    DEFINES *= RABBITIM_USE_QXMPP
    CONFIG(release, debug|release) {
        myPackagesExist(qxmpp) {
            MYPKGCONFIG *= qxmpp
        } else : msvc {
            LIBS += -lqxmpp0
        }
    } else {
        myPackagesExist(qxmpp_d) {
            MYPKGCONFIG *= qxmpp_d
        }else : msvc {
            LIBS += -lqxmpp_d0
        }
    }
}

equals(RABBITIM_USE_FFMPEG, 1) {
    DEFINES *= RABBITIM_USE_FFMPEG __STDC_CONSTANT_MACROS #ffmpeg需要  
    myPackagesExist(libavcodec libavformat libswscale libavutil) {
        MYPKGCONFIG *= libavcodec libavformat libswscale libavutil 
    } else : msvc {
        LIBS += -lavcodec -lavformat -lswscale -lavutil 
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

equals(QXMPP_USE_VPX, 1) {
    myPackagesExist(vpx){
        MYPKGCONFIG *= vpx
    } else : msvc {
        LIBS += -lvpx
    }
}

myPackagesExist(libqrencode) {
    DEFINES *= RABBITIM_USE_LIBQRENCODE
    MYPKGCONFIG *= libqrencode
}

myPackagesExist(QZXing) {
    DEFINES *= RABBITIM_USE_QZXING
    MYPKGCONFIG *= QZXing
} else : msvc {
    DEFINES *= RABBITIM_USE_QZXING
    LIBS += -lQZXing2
}

equals(RABBITIM_USE_LIBCURL, 1) {
    myPackagesExist(libcurl) {
        DEFINES *= RABBITIM_USE_LIBCURL
        MYPKGCONFIG *= libcurl
    } else : msvc {
        DEFINES *= RABBITIM_USE_LIBCURL
        LIBS += -llibcurl
    }
}

equals(RABBITIM_USE_OPENSSL, 1) {
    myPackagesExist(openssl) {
        DEFINES *= RABBITIM_USE_OPENSSL
        MYPKGCONFIG *= openssl
    } else : msvc {
        DEFINES *= RABBITIM_USE_OPENSSL
        LIBS += -llibeay32 -lssleay32 
    }
}

equals(RABBITIM_USE_WEBRTC, 1) {
    DEFINES *= RABBITIM_USE_WEBRTC
    
    msvc {
        DEFINES *= V8_DEPRECATION_WARNINGS CLD_VERSION=2 EXPAT_RELATIVE_PATH FEATURE_ENABLE_VOICEMAIL GTEST_RELATIVE_PATH JSONCPP_RELATIVE_PATH LOGGING=1 SRTP_RELATIVE_PATH FEATURE_ENABLE_SSL FEATURE_ENABLE_PSTN HAVE_SCTP HAVE_SRTP HAVE_WEBRTC_VIDEO HAVE_WEBRTC_VOICE NOMINMAX PSAPI_VERSION=1 CERT_CHAIN_PARA_HAS_EXTRA_FIELDS WIN32_LEAN_AND_MEAN CHROMIUM_BUILD CR_CLANG_REVISION=247874-1 USE_AURA=1 USE_ASH=1 USE_DEFAULT_RENDER_THEME=1 USE_LIBJPEG_TURBO=1 ENABLE_ONE_CLICK_SIGNIN ENABLE_PRE_SYNC_BACKUP ENABLE_WEBRTC=1 ENABLE_MEDIA_ROUTER=1 ENABLE_PEPPER_CDMS ENABLE_CONFIGURATION_POLICY ENABLE_NOTIFICATIONS ENABLE_HIDPI=1 ENABLE_TOPCHROME_MD=1 DONT_EMBED_BUILD_METADATA FIELDTRIAL_TESTING_ENABLED NO_TCMALLOC ALLOCATOR_SHIM ENABLE_TASK_MANAGER=1 ENABLE_EXTENSIONS=1 ENABLE_PDF=1 ENABLE_PLUGIN_INSTALLATION=1 ENABLE_PLUGINS=1 ENABLE_SESSION_SERVICE=1 ENABLE_THEMES=1 ENABLE_AUTOFILL_DIALOG=1 ENABLE_BACKGROUND=1 ENABLE_GOOGLE_NOW=1 ENABLE_PRINTING=1 ENABLE_BASIC_PRINTING=1 ENABLE_PRINT_PREVIEW=1 ENABLE_SPELLCHECK=1 ENABLE_CAPTIVE_PORTAL_DETECTION=1 ENABLE_APP_LIST=1 ENABLE_SETTINGS_APP=1 ENABLE_SUPERVISED_USERS=1 ENABLE_MDNS=1 ENABLE_SERVICE_DISCOVERY=1 V8_USE_EXTERNAL_STARTUP_DATA FULL_SAFE_BROWSING SAFE_BROWSING_CSD SAFE_BROWSING_DB_LOCAL WEBRTC_WIN USE_LIBPCI=1 USE_OPENSSL=1 NVALGRIND DYNAMIC_ANNOTATIONS_ENABLED=0
        QMAKE_CXXFLAGS *= 
        LIBS *= crypt32.lib iphlpapi.lib secur32.lib winmm.lib dmoguids.lib \
            wmcodecdspuuid.lib amstrmid.lib msdmo.lib Strmiids.lib
    }
    !isEmpty(WEBRTC_ROOT) {
        INCLUDEPATH += $${WEBRTC_ROOT}/include
        LIBS+=-L$${WEBRTC_ROOT}/lib
    } else {
        error("Please set WEBRTC_ROOT")
    }

    WEBRTC_LIBS = jingle_peerconnection \
        jingle_media \
        yuv \
        jpeg \
        vpx_new \
        vpx_intrinsics_mmx \
        vpx_intrinsics_sse2 \
        vpx_intrinsics_ssse3 \
        vpx_intrinsics_sse4_1 \
        vpx_intrinsics_avx2 \
        jingle_p2p \
        srtp 

    values = $$WEBRTC_LIBS
    for(var, values) {
        msvc {
            LIBS += lib$${var}
        }
    }

    WEBRTC_LIBS += \
        webrtc \
        field_trial_default \
        jsoncpp \
        rtc_base \
        webrtc_common \
        rtc_base_approved \
        boringssl \
        expat \
        video_render_module \
        webrtc_utility \
        audio_coding_module \
        cng \
        common_audio \
        system_wrappers \
        openmax_dl \
        common_audio_sse2 \
        audio_encoder_interface \
        g711 \
        pcm16b \
        webrtc_opus \
        opus \
        g722 \
        isac \
        audio_decoder_interface \
        isac_common \
        ilbc \
        red \
        rtc_event_log \
        rtc_event_log_proto \
        protobuf_lite \
        neteq \
        rent_a_codec  \
        media_file \
        common_video \
        voice_engine \
        audio_conference_mixer \
        audio_processing \
        audioproc_debug_proto \
        audio_processing_sse2 \
        audio_device \
        bitrate_controller \
        rtp_rtcp \
        remote_bitrate_estimator \
        paced_sender \
        video_capture_module \
        video_processing \
        video_processing_sse2 \
        webrtc_video_coding \
        webrtc_h264 \
        webrtc_i420 \
        video_coding_utility \
        webrtc_vp8 \
        webrtc_vp9 \
        rtc_sound \
        metrics_default \
        rtc_xmllite \
        rtc_xmpp \
        rtc_p2p \
        usrsctplib \
        video_capture_module_internal_impl \
        directshow_baseclasses \
        video_render_module_internal_impl 

    values = $$WEBRTC_LIBS
    for(var, values) {
             LIBS += -l$${var}
    }
}
