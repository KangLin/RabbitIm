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

qtHaveModule(webkit) {
    QT += webkitwidgets
    DEFINES += RABBITIM_WEBKIT 
}

lessThan(QT_VERSION, 5.0) : error("version is $$QT_VERSION, please qt is used greater then 5.0")

TARGET = RabbitIm
TEMPLATE = app

#设置目标输出目录  
win32{
    CONFIG(debug, debug|release) {
        TARGET_PATH=$${OUT_PWD}/Debug
    } else {
        TARGET_PATH=$${OUT_PWD}/Release
    }
}else{
    TARGET_PATH=$$OUT_PWD
}

DEFINES += RABBITIM
CONFIG += c++0x
!msvc{
    QMAKE_CXXFLAGS += " -std=c++0x "
}

CONFIG += link_pkgconfig link_prl

#安装  
isEmpty(PREFIX) {
    android {
       PREFIX = /.
    } else {
       PREFIX = $$OUT_PWD/$$TARGET
    } 
}

CONFIG(debug, debug|release) {
    #调试宏   
    DEFINES += DEBUG #DEBUG_VIDEO_TIME  
}

#android选项中包含了unix选项，所以在写工程如下条件判断时，必须把android条件放在unix条件前  
win32 {
    DEFINES += WINDOWS
    RABBITIM_SYSTEM = "windows"
    msvc {
        QMAKE_CXXFLAGS += /wd"4819"  #忽略msvc下对utf-8的警告  
        LDFLAGS += -ladvapi32
        RABBITIM_PLATFORM = "msvc"
        isEmpty(THIRD_LIBRARY_PATH) : THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/windows_msvc
        CONFIG(release, debug|release){
            LDFLAGS += /NODEFAULTLIB:libcmt /SUBSYSTEM:WINDOWS",5.01"
        } else:CONFIG(debug, debug|release) {
            LDFLAGS += /NODEFAULTLIB:libcmtd /SUBSYSTEM:WINDOWS",5.01"      
        }
    } else {
        RABBITIM_PLATFORM = "mingw"
        isEmpty(THIRD_LIBRARY_PATH) : THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/windows_mingw
    }
} else:android {
    isEmpty(THIRD_LIBRARY_PATH) : THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/android
    PKG_CONFIG_SYSROOT_DIR=$${THIRD_LIBRARY_PATH}
    DEFINES += ANDROID MOBILE
    RABBITIM_SYSTEM = "android"
}  else:unix {
    RABBITIM_SYSTEM = unix
    DEFINES += UNIX
    isEmpty(THIRD_LIBRARY_PATH) : THIRD_LIBRARY_PATH = $$PWD/ThirdLibary/unix
}
equals(RABBITIM_USE_STATIC, 1) {
    exists("$${THIRD_LIBRARY_PATH}_static"){
        THIRD_LIBRARY_PATH=$${THIRD_LIBRARY_PATH}_static
    }
    CONFIG *= link_prl static #生成静态程序  
   # CONFIG += staticlib # this is needed if you create a static library, not a static executable
   # DEFINES+= STATIC
#} else {
#    CONFIG += staticlib #生成静态库    
#    CONFIG += shared    #生成动态库  
}
message("THIRD_LIBRARY_PATH:$${THIRD_LIBRARY_PATH}")

INCLUDEPATH += $$PWD $$PWD/Widgets/FrmCustom
INCLUDEPATH += $${THIRD_LIBRARY_PATH}/include
DEPENDPATH += $${THIRD_LIBRARY_PATH}/include
LIBS += -L$${THIRD_LIBRARY_PATH}/lib $$LDFLAGS

include(mkspecs/ThirdLibrary.prf)
include(RabbitIm.pri)
include(Plugin/Lbs/Lbs.pri)
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
    Doxyfile* \
    Plugin/CMakeLists.txt \
    mkspecs/*

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
    #QMAKE_EXTRA_TARGETS += Deployment_qtlib Deployment_third_lib
    
    #复制第三方依赖库动态库到编译输出目录  
    THIRD_LIBRARY_DLL = $${THIRD_LIBRARY_PATH}/bin/*.dll
    exists($${THIRD_LIBRARY_DLL}){
        equals(QMAKE_HOST.os, Windows):isEmpty(QMAKE_SH){
            THIRD_LIBRARY_DLL = $$system_path($$THIRD_LIBRARY_DLL)
            TARGET_PATH = $$system_path($$TARGET_PATH)
        }
        ThirdLibraryDll.commands = \
            $${QMAKE_COPY} $${THIRD_LIBRARY_DLL} $${TARGET_PATH}
        ThirdLibraryDll.CONFIG += directory no_link no_clean no_check_exist
        ThirdLibraryDll.target = ThirdLibraryDll
        QMAKE_EXTRA_TARGETS += ThirdLibraryDll
        POST_TARGETDEPS += ThirdLibraryDll
    }

    !exists($${TARGET_PATH}/platforms):equals(QMAKE_HOST.os, Windows){
        PlatformsPlugins.commands = \
            $(COPY_DIR) $$system_path($$[QT_INSTALL_PLUGINS]/platforms) $$system_path($${TARGET_PATH}/platforms)
        PlatformsPlugins.CONFIG += directory no_link no_clean no_check_exist
        PlatformsPlugins.target = PlatformsPlugins
        QMAKE_EXTRA_TARGETS += PlatformsPlugins
        POST_TARGETDEPS += PlatformsPlugins
    }
}
