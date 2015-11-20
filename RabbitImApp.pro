#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

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

LIBS += -L$${TARGET_PATH}
include(pri/ThirdLibraryConfig.pri)
include(pri/ThirdLibrary.pri)
MYPKGCONFIG *= RabbitIm
include(pri/ThirdLibraryJoin.pri)

SOURCES += main.cpp

#发行版本才更新更新配置  
CONFIG(release, debug|release) {
    include(pri/RabbitImOther.pri)
    !equals(RABBITIM_USE_LIBCURL, 1){
        warning("don't update function")
    }
}

CONFIG += mobility

MOBILITY = 

#应用程序图标  
RC_FILE = AppIcon.rc

CONFIG += localize_deployment  #本地语言部署  

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
            $$PWD/ThirdLibrary/android/lib/libnative_camera_r4.2.0.so  \ #TODO:修改成你手机平台对应的版本，如果没有，则取最近的版本  
            $$PWD/ThirdLibrary/android/lib/libopencv_info.so #\
            #$$PWD/ThirdLibrary/android/lib/libopencv_core.so \
            #$$PWD/ThirdLibrary/android/lib/libopencv_imgproc.so \
            #$$PWD/ThirdLibrary/android/lib/libopencv_videoio.so \
            #$$PWD/ThirdLibrary/android/lib/libopencv_flann.so \
            #$$PWD/ThirdLibrary/android/lib/libopencv_imgcodecs.so \
            #$$PWD/ThirdLibrary/android/lib/libopencv_video.so
            #$$PWD/ThirdLibrary/android/lib/libnative_camera_r2.2.0.so \
            #$$PWD/ThirdLibrary/android/lib/libnative_camera_r4.1.1.so \
            #$$PWD/ThirdLibrary/android/lib/libnative_camera_r2.3.3.so \
            #$$PWD/ThirdLibrary/android/lib/libnative_camera_r3.0.1.so \
            #$$PWD/ThirdLibrary/android/lib/libnative_camera_r4.3.0.so \
            #$$PWD/ThirdLibrary/android/lib/libnative_camera_r4.0.0.so \
            #$$PWD/ThirdLibrary/android/lib/libnative_camera_r4.4.0.so \
            #$$PWD/ThirdLibrary/android/lib/libnative_camera_r4.0.3.so \
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
