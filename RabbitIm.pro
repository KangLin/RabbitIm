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
    circle.yml \
    appveyor.yml \
    debian/* \
    ThirdLibrary/build_script/ci/*.sh \
    ThirdLibrary/build_script/*.sh \
    ThirdLibrary/*.yml \
    ThirdLibrary/*.md \
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
    pri/* \
    Dockerfile 
