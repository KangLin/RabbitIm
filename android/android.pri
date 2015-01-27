ANDROID_PACKAGE_SOURCE_DIR = $$PWD  #ANDROID包的源码目录  

include(jni/android_jni.pri)

OTHER_FILES +=  \
    $$PWD/*  \
    $$PWD/src/org/KangLinStudio/RabbitIm/*
