#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

TARGET = RabbitIm
TEMPLATE = lib
!CONFIG(static, static|shared) : DEFINES += BUILD_SHARED_LIBS #windows下动态库导出

CONFIG += create_prl no_install_prl create_pc no_install_pc 
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

#设置目标输出目录
isEmpty(DESTDIR): DESTDIR = $$OUT_PWD
win32: DEFINES += DLL_EXPORT

#安装前缀  
isEmpty(PREFIX) {
    android {
        PREFIX = /.
    } else {
        PREFIX = $$DESTDIR/install
    } 
}

#修改文件中的第三方库配置  
include(pri/ThirdLibraryConfig.pri)
include(pri/ThirdLibrary.pri)
include(pri/ThirdLibraryJoin.pri)
include(pri/RabbitImFiles.pri)

#发行版本才更新更新配置  
include(pri/RabbitImVersion.pri)

#翻译资源
isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$(RabbitCommon_DIR)
isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$PWD/../RabbitCommon
!isEmpty(RabbitCommon_DIR): exists("$${RabbitCommon_DIR}/Src/Src.pro") {
    include($${RabbitCommon_DIR}/pri/Translations.pri)
} else {
    message("Don't find RabbitCommon, in environment variable RabbitCommon_DIR:$$RabbitCommon_DIR")
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon ag:")
    message("   git clone https://github.com/KangLin/RabbitCommon.git")
    error  ("2. Then set environment variable RabbitCommon_DIR to download dirctory")
}

target.path = $$PREFIX
!android : INSTALLS += target
#android : CONFIG += static   #TODO：android < 18时，动态库加载会失败（可能是有未支持的函数），原因不明   

#ANDROID 平台相关内容  
android : include(android/jni/android_jni.pri)

win32:equals(QMAKE_HOST.os, Windows){

    INSTALL_TARGET = $$system_path($${PREFIX}/$(TARGET))  #$(TARGET)是qmake产生脚本中的引用

    #mingw{  #手机平台不需要  
    #    RABBITIM_STRIP.target = RABBITIM_STRIP
    #    RABBITIM_STRIP.commands = "strip $$INSTALL_TARGET"
    #    INSTALLS += RABBITIM_STRIP
    #}
    #安装qt依赖库  
    Deployment_qtlib.target = Deployment_qtlib
    Deployment_qtlib.path = $$system_path($${PREFIX})
    Deployment_qtlib.commands = "$$system_path($$[QT_INSTALL_BINS]/windeployqt)" \
                    --compiler-runtime \
                    --verbose 7 \
                    "$${INSTALL_TARGET}"
    
    INSTALLS += Deployment_qtlib
}
