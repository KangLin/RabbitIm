#App 插件的公共配置，如果你是 App 插件，需要包含此文件

CONFIG *= plugin 
INCLUDEPATH += $$PWD/..
CONFIG += c++0x
!msvc{
    QMAKE_CXXFLAGS += " -std=c++0x "
} else {
    QMAKE_CXXFLAGS += " /MP "
}

TARGET_PATH=$$OUT_PWD/../..
#设置目标输出目录  
win32{
    CONFIG(debug, debug|release) {
        TARGET_PATH=$${TARGET_PATH}/Debug
    } else {
        TARGET_PATH=$${TARGET_PATH}/Release
    }
}
LIBS += -L$${TARGET_PATH}   #包含 RabbitIm 库位置  
!exists("$$OUT_PWD") : mkpath($$OUT_PWD)
#message("TARGET_PATH:$${TARGET_PATH}")

include($$PWD/../pri/ThirdLibraryConfig.pri)
myPackagesExist(RabbitIm){
    MYPKGCONFIG *= RabbitIm 
}else : msvc {
    LIBS += -lRabbitIm0
}
include($$PWD/../pri/ThirdLibrary.pri)
include($$PWD/../pri/ThirdLibraryJoin.pri)

#安装前缀  
isEmpty(PREFIX) {
    android {
       PREFIX = /.
    } else {
        PREFIX = $$OUT_PWD/../../install
    } 
}
contains(TEMPLATE, lib){

    CONFIG += create_prl link_prl #create_pc no_install_pc no_install_prl
    #QMAKE_PKGCONFIG_DESTDIR = ../pkgconfig

    #为静态插件生成必要的文件  
    CONFIG(static, static|shared) {
        isEmpty(RABBITIM_PLUG_NAME) : error("Please set RABBITIM_PLUG_NAME to plug class name")
        FILE_NAME=$$PWD/PluginStatic.cpp
        PLUG_CONTENT = "Q_IMPORT_PLUGIN($${RABBITIM_PLUG_NAME})"
        FILE_CONTENT = $$cat($$FILE_NAME)
        !contains(FILE_CONTENT, $$PLUG_CONTENT){
            PLUG_CONTENT = "    Q_IMPORT_PLUGIN($${RABBITIM_PLUG_NAME})"
            write_file($$FILE_NAME, PLUG_CONTENT, append)
        }
    
        FILE_NAME=$$PWD/PluginStatic.pri
        PLUG_CONTENT = "-l$${TARGET}"
        FILE_CONTENT = $$cat($$FILE_NAME) 
        !contains(FILE_CONTENT, $$PLUG_CONTENT){
            PLUG_CONTENT = "LIBS *= -L\$\${OUT_PWD}/plugins/App/$${TARGET} -l$${TARGET} "
            #PLUG_CONTENT += "myPackagesExist($${TARGET}) : MYPKGCONFIG *= $${TARGET}"
            write_file($$FILE_NAME, PLUG_CONTENT, append)
        }
    }

    #插件安装路径  
    DESTDIR = $$OUT_PWD/../../plugins/App/$${TARGET}
    mkpath($$DESTDIR)

    #插件安装路径  
    TARGET_INSTALL_PATH = $${PREFIX}/plugins/App/$${TARGET}
    target.path = $${TARGET_INSTALL_PATH}

    #翻译  
    include($$PWD/translations.pri)
} else {
    target.path = $$PREFIX
    DESTDIR = $$TARGET_PATH
}

!android : INSTALLS += target

win32:equals(QMAKE_HOST.os, Windows){
    #isEmpty(QMAKE_SH){
       INSTALL_TARGET = $$system_path($${TARGET_INSTALL_PATH}/$(TARGET))  #$(TARGET)是qmake产生脚本中的引用
    #} else {
    #    INSTALL_TARGET = $${PREFIX}/$(TARGET)
    #}
    #mingw{  #手机平台不需要  
    #    RABBITIM_STRIP.target = RABBITIM_STRIP
    #    RABBITIM_STRIP.commands = "strip $$INSTALL_TARGET"
    #    INSTALLS += RABBITIM_STRIP
    #}
    #安装qt依赖库  
    Deployment_qtlib.target = Deployment_qtlib
    Deployment_qtlib.path = $$system_path($${PREFIX})
    Deployment_qtlib.commands = "$$system_path($$[QT_INSTALL_BINS]/windeployqt)" \
                        --dir "$$system_path($${PREFIX})" \
                        --compiler-runtime \
                        --verbose 7 \
                        "$${INSTALL_TARGET}"
        
    INSTALLS += Deployment_qtlib
}
