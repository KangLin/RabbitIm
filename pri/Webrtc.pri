#RABBITIM_USE_WEBRTC=1        #使用 webrtc 库  

equals(RABBITIM_USE_WEBRTC, 1) {
    DEFINES *= RABBITIM_USE_WEBRTC

    !isEmpty(WEBRTC_ROOT) {
        INCLUDEPATH += $${WEBRTC_ROOT}/include
        LIBS+=-L$${WEBRTC_ROOT}/lib
    } else {
        message("Please set WEBRTC_ROOT")
    }

    msvc {
        DEFINES *= V8_DEPRECATION_WARNINGS CLD_VERSION=2 EXPAT_RELATIVE_PATH FEATURE_ENABLE_VOICEMAIL GTEST_RELATIVE_PATH JSONCPP_RELATIVE_PATH LOGGING=1 SRTP_RELATIVE_PATH FEATURE_ENABLE_SSL FEATURE_ENABLE_PSTN HAVE_SCTP HAVE_SRTP HAVE_WEBRTC_VIDEO HAVE_WEBRTC_VOICE NOMINMAX PSAPI_VERSION=1 CERT_CHAIN_PARA_HAS_EXTRA_FIELDS WIN32_LEAN_AND_MEAN CHROMIUM_BUILD CR_CLANG_REVISION=247874-1 USE_AURA=1 USE_ASH=1 USE_DEFAULT_RENDER_THEME=1 USE_LIBJPEG_TURBO=1 ENABLE_ONE_CLICK_SIGNIN ENABLE_PRE_SYNC_BACKUP ENABLE_WEBRTC=1 ENABLE_MEDIA_ROUTER=1 ENABLE_PEPPER_CDMS ENABLE_CONFIGURATION_POLICY ENABLE_NOTIFICATIONS ENABLE_HIDPI=1 ENABLE_TOPCHROME_MD=1 DONT_EMBED_BUILD_METADATA FIELDTRIAL_TESTING_ENABLED NO_TCMALLOC ALLOCATOR_SHIM ENABLE_TASK_MANAGER=1 ENABLE_EXTENSIONS=1 ENABLE_PDF=1 ENABLE_PLUGIN_INSTALLATION=1 ENABLE_PLUGINS=1 ENABLE_SESSION_SERVICE=1 ENABLE_THEMES=1 ENABLE_AUTOFILL_DIALOG=1 ENABLE_BACKGROUND=1 ENABLE_GOOGLE_NOW=1 ENABLE_PRINTING=1 ENABLE_BASIC_PRINTING=1 ENABLE_PRINT_PREVIEW=1 ENABLE_SPELLCHECK=1 ENABLE_CAPTIVE_PORTAL_DETECTION=1 ENABLE_APP_LIST=1 ENABLE_SETTINGS_APP=1 ENABLE_SUPERVISED_USERS=1 ENABLE_MDNS=1 ENABLE_SERVICE_DISCOVERY=1 V8_USE_EXTERNAL_STARTUP_DATA FULL_SAFE_BROWSING SAFE_BROWSING_CSD SAFE_BROWSING_DB_LOCAL WEBRTC_WIN USE_LIBPCI=1 USE_OPENSSL=1 NVALGRIND DYNAMIC_ANNOTATIONS_ENABLED=0
        LIBS *= crypt32.lib iphlpapi.lib secur32.lib winmm.lib dmoguids.lib \
            wmcodecdspuuid.lib amstrmid.lib msdmo.lib Strmiids.lib

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
                LIBS += lib$${var}.lib
            }
        }
    
        WEBRTC_LIBS = \
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

        for(var, WEBRTC_LIBS) {
            LIBS += $${var}.lib
        }
    }

    unix {
        CONFIG(debug, debug|release) {
             DEFINES *= V8_DEPRECATION_WARNINGS CLD_VERSION=2 EXPAT_RELATIVE_PATH \
                    FEATURE_ENABLE_VOICEMAIL GTEST_RELATIVE_PATH \
                    JSONCPP_RELATIVE_PATH LOGGING=1 SRTP_RELATIVE_PATH \
                    FEATURE_ENABLE_SSL FEATURE_ENABLE_PSTN HAVE_SCTP HAVE_SRTP \
                    HAVE_WEBRTC_VIDEO HAVE_WEBRTC_VOICE _FILE_OFFSET_BITS=64 \
                    CHROMIUM_BUILD CR_CLANG_REVISION=247874-1 \
                    UI_COMPOSITOR_IMAGE_TRANSPORT USE_AURA=1 USE_ASH=1 USE_PANGO=1 \
                    USE_CAIRO=1 USE_DEFAULT_RENDER_THEME=1 USE_LIBJPEG_TURBO=1 \
                    USE_X11=1 USE_CLIPBOARD_AURAX11=1 ENABLE_ONE_CLICK_SIGNIN \
                    ENABLE_PRE_SYNC_BACKUP ENABLE_WEBRTC=1 ENABLE_MEDIA_ROUTER=1 \
                    ENABLE_PEPPER_CDMS ENABLE_CONFIGURATION_POLICY \
                    ENABLE_NOTIFICATIONS ENABLE_HIDPI=1 ENABLE_TOPCHROME_MD=1 \
                    USE_UDEV DONT_EMBED_BUILD_METADATA FIELDTRIAL_TESTING_ENABLED \
                    ENABLE_TASK_MANAGER=1 ENABLE_EXTENSIONS=1 ENABLE_PDF=1 \
                    ENABLE_PLUGINS=1 ENABLE_SESSION_SERVICE=1 ENABLE_THEMES=1 \
                    ENABLE_AUTOFILL_DIALOG=1 ENABLE_BACKGROUND=1 ENABLE_GOOGLE_NOW=1 \
                    ENABLE_PRINTING=1 ENABLE_BASIC_PRINTING=1 ENABLE_PRINT_PREVIEW=1 \
                    ENABLE_SPELLCHECK=1 ENABLE_CAPTIVE_PORTAL_DETECTION=1 \
                    ENABLE_APP_LIST=1 ENABLE_SETTINGS_APP=1 ENABLE_SUPERVISED_USERS=1 \
                    ENABLE_MDNS=1 ENABLE_SERVICE_DISCOVERY=1 \
                    V8_USE_EXTERNAL_STARTUP_DATA FULL_SAFE_BROWSING SAFE_BROWSING_CSD \
                    SAFE_BROWSING_DB_LOCAL LINUX WEBRTC_LINUX \
                    HASH_NAMESPACE=__gnu_cxx WEBRTC_POSIX DISABLE_DYNAMIC_CAST \
                    _REENTRANT USE_LIBPCI=1 USE_OPENSSL=1 USE_GLIB=1 \
                    USE_NSS_CERTS=1 DYNAMIC_ANNOTATIONS_ENABLED=1 \
                    WTF_USE_DYNAMIC_ANNOTATIONS=1 _DEBUG _GLIBCXX_DEBUG=1
            } else {
                DEFINES *= V8_DEPRECATION_WARNINGS CLD_VERSION=2 EXPAT_RELATIVE_PATH \
                    FEATURE_ENABLE_VOICEMAIL GTEST_RELATIVE_PATH \
                    JSONCPP_RELATIVE_PATH LOGGING=1 SRTP_RELATIVE_PATH \
                    FEATURE_ENABLE_SSL FEATURE_ENABLE_PSTN HAVE_SCTP HAVE_SRTP \
                    HAVE_WEBRTC_VIDEO HAVE_WEBRTC_VOICE _FILE_OFFSET_BITS=64 \
                    CHROMIUM_BUILD CR_CLANG_REVISION=247874-1 \
                    UI_COMPOSITOR_IMAGE_TRANSPORT USE_AURA=1 USE_ASH=1 USE_PANGO=1 \
                    USE_CAIRO=1 USE_DEFAULT_RENDER_THEME=1 USE_LIBJPEG_TURBO=1 \
                    USE_X11=1 USE_CLIPBOARD_AURAX11=1 ENABLE_ONE_CLICK_SIGNIN \
                    ENABLE_PRE_SYNC_BACKUP ENABLE_WEBRTC=1 ENABLE_MEDIA_ROUTER=1 \
                    ENABLE_PEPPER_CDMS ENABLE_CONFIGURATION_POLICY \
                    ENABLE_NOTIFICATIONS ENABLE_HIDPI=1 ENABLE_TOPCHROME_MD=1 \
                    USE_UDEV DONT_EMBED_BUILD_METADATA FIELDTRIAL_TESTING_ENABLED \
                    ENABLE_TASK_MANAGER=1 ENABLE_EXTENSIONS=1 ENABLE_PDF=1 \
                    ENABLE_PLUGINS=1 ENABLE_SESSION_SERVICE=1 ENABLE_THEMES=1 \
                    ENABLE_AUTOFILL_DIALOG=1 ENABLE_BACKGROUND=1 ENABLE_GOOGLE_NOW=1 \
                    ENABLE_PRINTING=1 ENABLE_BASIC_PRINTING=1 ENABLE_PRINT_PREVIEW=1 \
                    ENABLE_SPELLCHECK=1 ENABLE_CAPTIVE_PORTAL_DETECTION=1 \
                    ENABLE_APP_LIST=1 ENABLE_SETTINGS_APP=1 ENABLE_SUPERVISED_USERS=1 \
                    ENABLE_MDNS=1 ENABLE_SERVICE_DISCOVERY=1 \
                    V8_USE_EXTERNAL_STARTUP_DATA FULL_SAFE_BROWSING SAFE_BROWSING_CSD \
                    SAFE_BROWSING_DB_LOCAL LINUX WEBRTC_LINUX \
                    HASH_NAMESPACE=__gnu_cxx WEBRTC_POSIX DISABLE_DYNAMIC_CAST \
                    _REENTRANT USE_LIBPCI=1 USE_OPENSSL=1 USE_GLIB=1 \
                    USE_NSS_CERTS=1 NDEBUG NVALGRIND DYNAMIC_ANNOTATIONS_ENABLED=0
            }

             WEBRTC_LIBS = \
                        jingle_peerconnection \
                        field_trial_default \
                        jsoncpp \
                        video_render_module \
                        cng \
                        openmax_dl \
                        ilbc \
                        red \
                        media_file \
                        video_capture_module_internal_impl \
                        video_render_module_internal_impl \
                        video_capture_module \
                        video_processing \
                        video_processing_sse2 \
                        vpx_new \
                        vpx_intrinsics_mmx \
                        vpx_intrinsics_sse2 \
                        vpx_intrinsics_ssse3 \
                        vpx_intrinsics_sse4_1 \
                        vpx_intrinsics_avx2 \
                        rtc_xmllite \
                        rtc_xmpp  \
                        jingle_p2p \
                        rtc_p2p \
                        jingle_media \
                        webrtc \
                        webrtc_video_coding \
                        webrtc_i420 \
                        webrtc_vp8 \
                        webrtc_vp9 \
                        webrtc_h264 \
                        video_coding_utility \
                        voice_engine \
                        audio_coding_module \
                        audio_processing \
                        audio_conference_mixer \
                        audioproc_debug_proto \
                        audio_processing_sse2 \
                        audio_device \
                        webrtc_utility \
                        paced_sender \
                        neteq \
                        rent_a_codec \
                        isac_common \
                        g711 pcm16b \
                        webrtc_opus \
                        opus \
                        g722 isac \
                        audio_encoder_interface \
                        audio_decoder_interface \
                        bitrate_controller \
                        remote_bitrate_estimator \
                        metrics_default \
                        rtp_rtcp \
                        rtc_sound \
                        rtc_event_log \
                        rtc_event_log_proto \
                        protobuf_lite \
                        webrtc_common \
                        common_video \
                        common_audio \
                        common_audio_sse2 \
                        system_wrappers \
                        yuv \
                        jpeg_turbo \
                        rtc_base \
                        rtc_base_approved \
                        srtp \
                        usrsctplib \
                        boringssl 

        for(var, WEBRTC_LIBS) {
            LIBS += -l$${var}
        }

        LIBS += -lgthread-2.0 -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 \
            -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 \
            -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lX11 -lXcomposite \
            -lXext -lXrender -ldl -lrt -lexpat -lm

    }
}
