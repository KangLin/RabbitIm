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

CONFIG += create_pc create_prl link_prl
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

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

equals(QXMPP_USE_VPX, 1) : packagesExist(vpx){
    android {
        LIBS += -lcpu-features
    }
}

include(pri/RabbitImFiles.pri)

CONFIG += mobility

MOBILITY = 

#应用程序图标  
RC_FILE = AppIcon.rc

CONFIG += localize_deployment  #本地语言部署  

OTHER_FILES += README.md \
    .gitignore \
    .travis.yml \
    debian/* \
    ThirdLibrary/build_script/travis/*.sh \
    ThirdLibrary/build_script/*.sh \
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
    pri/*

#ANDROID 平台相关内容  
android{
    include(android/android.pri)
}
