ANDROID_PACKAGE_SOURCE_DIR += $$PWD  #ANDROID包的源码目录  

include(jni/android_jni.pri)

OTHER_FILES +=  \
    $$PWD/*  \
    $$PWD/src/org/KangLinStudio/RabbitIm/* \
    $$PWD/src/org/pjsip/* \
    $$PWD/res/values/* \
    $$PWD/res/values-zh/* \
    $$PWD/res/values-zh-rCN/* \
    $$PWD/res/values-zh-rTW/* \
    $$PWD/res/*

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

#安装 android 支持库  
android_support_library.target = android_support_library
android_support_library.path = $$PREFIX/libs
equals(QMAKE_HOST.os, Windows){
    isEmpty(QMAKE_SH){
        androidsupportlibraryfiles = $$system_path($$(ANDROID_HOME)/extras/android/support/v4/android-support-v4.jar)
    }
}else{
    androidsupportlibraryfiles = $$(ANDROID_HOME)/extras/android/support/v4/android-support-v4.jar
}
android_support_library.files = $$androidsupportlibraryfiles
message("androidsupportlibraryfiles:$$androidsupportlibraryfiles")
INSTALLS += android_support_library

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
    ANDROID_EXTRA_LIBS +=$$(ANDROID_NDK_ROOT)/platforms/$$(ANDROID_NDK_PLATFORM)/arch-arm/usr/lib/libc.so
}
