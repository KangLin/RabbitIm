#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

TARGET = RabbitIm
TEMPLATE = lib 

CONFIG += create_pc create_prl no_install_pc no_install_prl
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

#发行版本才更新更新配置  
CONFIG(release, debug|release) {
    include(pri/RabbitImVersion.pri)
    !equals(RABBITIM_USE_LIBCURL, 1){
        warning("don't update function")
    }
}

VERSION = $${MAJOR_VERSION_NUMBER}.$${MINOR_VERSION_NUMBER}.$${REVISION_VERSION_NUMBER}

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

#修改文件中的第三方库配置  
include(pri/ThirdLibraryConfig.pri)
include(pri/ThirdLibrary.pri)
include(pri/ThirdLibraryJoin.pri)
include(pri/RabbitImFiles.pri)

# Rules for creating/updating {ts|qm}-files
include(Resource/translations/translations.pri)

target.path = $$PREFIX
!android : INSTALLS += target

CONFIG += mobility

MOBILITY = 

#ANDROID 平台相关内容  
android : include(android/jni/android_jni.pri)

win32{
    #安装qt依赖库  
    Deployment_qtlib.target = Deployment_qtlib
    Deployment_qtlib.path = $${PREFIX}
    Deployment_qtlib.commands = "$$[QT_INSTALL_BINS]/windeployqt" \
                    --compiler-runtime \
                    --verbose 7 \
                    "$${PREFIX}/${TARGET}"
    INSTALLS += Deployment_qtlib
}
