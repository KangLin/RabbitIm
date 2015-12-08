TEMPLATE = subdirs

RabbitImLib.file = RabbitImLib.pro
RabbitImApp.file = RabbitImApp.pro
Plugin.depends = RabbitImLib
RabbitImApp.depends = RabbitImLib Plugin

CONFIG *= ordered
SUBDIRS +=  RabbitImLib \
    Plugin \
    RabbitImApp

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
