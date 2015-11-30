TEMPLATE = subdirs

RabbitImLib.file = RabbitImLib.pro
RabbitImApp.file = RabbitImApp.pro
Plugin.depends = RabbitImLib
RabbitImApp.depends = RabbitImLib Plugin

CONFIG *= ordered
SUBDIRS +=  RabbitImLib \
    Plugin \
    RabbitImApp

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
