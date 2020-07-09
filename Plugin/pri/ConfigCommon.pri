CONFIG *= plugin 

win32: TARGET_PATH=$$OUT_PWD/../../bin
else: TARGET_PATH=$$OUT_PWD/../../lib
#设置目标输出目录  
!exists("$$OUT_PWD") : mkpath($$OUT_PWD)
#message("TARGET_PATH:$${TARGET_PATH}")
!CONFIG(static, static|shared) : DEFINES += BUILD_SHARED_LIBS #windows下动态库导出

include($$PWD/../../pri/ThirdLibraryConfig.pri)
INCLUDEPATH *= .. ../../Src
LIBS *=  -L$${TARGET_PATH} -lRabbitIm  #包含 RabbitIm 库位置

include($$PWD/../../pri/ThirdLibrary.pri)
include($$PWD/../../pri/ThirdLibraryJoin.pri)
include($$PWD/../../pri/RabbitImVersion.pri)

#安装前缀  
isEmpty(PREFIX) {
    android {
       PREFIX = /.
    } else {
        PREFIX = $$OUT_PWD/../../install
    }
}

isEmpty(PLUGIN_TYPE){
    error("Don't include ConfigCommon.pri directly, Please include ConfigApp.pri or ConfigProtocol.pri")
}

contains(TEMPLATE, lib){ #生成库

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
            PLUG_CONTENT = "LIBS *= -L\$\${OUT_PWD}/plugins/$${PLUGIN_TYPE}/$${TARGET} -l$${TARGET} "
            #PLUG_CONTENT += "myPackagesExist($${TARGET}) : MYPKGCONFIG *= $${TARGET}"
            write_file($$FILE_NAME, PLUG_CONTENT, append)
        }
    }

    #插件生成路径
    DESTDIR = $$OUT_PWD/../../plugins/$${PLUGIN_TYPE}/$${TARGET}
    mkpath($$DESTDIR)

    #插件安装路径  
    TARGET_INSTALL_PATH = $${PREFIX}/plugins/$${PLUGIN_TYPE}/$${TARGET}

    #翻译  
    include(translations.pri)
} else { #生成App
    TARGET_INSTALL_PATH = $$PREFIX/bin
    DESTDIR = $$TARGET_PATH
}

target.path = $${TARGET_INSTALL_PATH}
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
    Deployment_qtlib.path = $$system_path($${TARGET_INSTALL_PATH})
    Deployment_qtlib.commands = "$$system_path($$[QT_INSTALL_BINS]/windeployqt)" \
                        --dir "$$system_path($${TARGET_INSTALL_PATH})" \
                        --compiler-runtime \
                        --verbose 7 \
                        "$${INSTALL_TARGET}"
        
    INSTALLS += Deployment_qtlib
}
