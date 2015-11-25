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
TEMPLATE = lib 

android : CONFIG *= static
CONFIG += create_pc create_prl link_prl
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

#发行版本才更新更新配置  
CONFIG(release, debug|release) {
    include(pri/RabbitImOther.pri)
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

CONFIG += mobility

MOBILITY = 

#ANDROID 平台相关内容  
android{
    include(android/android.pri)
}
