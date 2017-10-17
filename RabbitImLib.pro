#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

TARGET = RabbitIm
TEMPLATE = lib 
!CONFIG(static, static|shared) : DEFINES += RABBITIM_SHARED_LIBRARY #windows下动态库导出

CONFIG += create_prl no_install_prl create_pc no_install_pc 
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

#设置目标输出目录  
win32{
    CONFIG(debug, debug|release)  {
        TARGET_PATH=$${OUT_PWD}/Debug
    } else {
        TARGET_PATH=$${OUT_PWD}/Release
    }
}else{
    TARGET_PATH=$$OUT_PWD
}

#安装前缀  
isEmpty(PREFIX) {
    android {
       PREFIX = /.
    } else {
        PREFIX = $$OUT_PWD/install
    } 
}

#修改文件中的第三方库配置  
include(pri/ThirdLibraryConfig.pri)
include(pri/ThirdLibrary.pri)
include(pri/ThirdLibraryJoin.pri)
include(pri/RabbitImFiles.pri)

#发行版本才更新更新配置  
include(pri/RabbitImVersion.pri)
!equals(RABBITIM_USE_LIBCURL, 1){
    warning("don't update function")
}

VERSION = $${MAJOR_VERSION_NUMBER}.$${MINOR_VERSION_NUMBER}.$${REVISION_VERSION_NUMBER}

# Rules for creating/updating {ts|qm}-files
include(Resource/translations/translations.pri)

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
