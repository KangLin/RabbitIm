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
