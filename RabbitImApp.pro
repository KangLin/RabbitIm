#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

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
CONFIG(static, static|shared) : include($$PWD/Plugin/PluginStatic.pri)
include(pri/ThirdLibraryConfig.pri)
myPackagesExist(RabbitIm) : MYPKGCONFIG *= RabbitIm
include(pri/ThirdLibrary.pri)
include(pri/ThirdLibraryJoin.pri)

SOURCES += main.cpp

CONFIG += mobility

MOBILITY = 

#应用程序图标  
RC_FILE = AppIcon.rc

CONFIG += localize_deployment  #本地语言部署  

other.files = License.md Authors.txt ChangeLog.md
other.path = $$PREFIX
other.CONFIG += directory no_check_exist 
target.path = $$PREFIX
INSTALLS += other target

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
    #mingw{  #手机平台不需要  
    #    RABBITIM_STRIP.target = RABBITIM_STRIP
    #    RABBITIM_STRIP.commands = "strip $${PREFIX}/${TARGET}"
    #}
    #安装qt依赖库  
    Deployment_qtlib.target = Deployment_qtlib
    Deployment_qtlib.path = $${PREFIX}
    Deployment_qtlib.commands = "$$[QT_INSTALL_BINS]/windeployqt" \
                    --compiler-runtime \
                    --verbose 7 \
                    "$${PREFIX}/${TARGET}"
                    #$$join(QT, ' -', -) "$${PREFIX}/$${TARGET}.exe"
    #安装第三方依赖库  
    Deployment_third_lib.target = Deployment_third_lib
    Deployment_third_lib.files = $${THIRD_LIBRARY_PATH}/lib/*.dll
    Deployment_third_lib.path = $$PREFIX
    Deployment_third_lib.CONFIG += directory no_check_exist

    Deployment_third_bin.target = Deployment_third_bin
    Deployment_third_bin.files = $${THIRD_LIBRARY_PATH}/bin/*.dll
    Deployment_third_bin.path = $$PREFIX
    Deployment_third_bin.CONFIG += directory no_check_exist
    INSTALLS += RABBITIM_STRIP Deployment_qtlib Deployment_third_lib Deployment_third_bin
    #QMAKE_EXTRA_TARGETS += Deployment_qtlib Deployment_third_lib Deployment_third_bin
    
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

    THIRD_LIBRARY_LIB = $${THIRD_LIBRARY_PATH}/lib/*.dll
    exists($${THIRD_LIBRARY_LIB}){
        equals(QMAKE_HOST.os, Windows):isEmpty(QMAKE_SH){
            THIRD_LIBRARY_LIB = $$system_path($$THIRD_LIBRARY_LIB)
            TARGET_PATH = $$system_path($$TARGET_PATH)
        }
        ThirdLibraryLib.commands = \
            $${QMAKE_COPY} $${THIRD_LIBRARY_LIB} $${TARGET_PATH}
        ThirdLibraryLib.CONFIG += directory no_link no_clean no_check_exist
        ThirdLibraryLib.target = ThirdLibraryLib
        QMAKE_EXTRA_TARGETS += ThirdLibraryLib
        POST_TARGETDEPS += ThirdLibraryLib
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
