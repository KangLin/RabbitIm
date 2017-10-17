#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

TARGET = RabbitImApp
TEMPLATE = app 

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
LIBS += -L$${TARGET_PATH}

include(pri/ThirdLibraryConfig.pri)
CONFIG(static, static|shared) {
    CONFIG *= link_prl
    include($$PWD/Plugin/PluginStatic.pri)
}
myPackagesExist(RabbitIm) {
    MYPKGCONFIG *= RabbitIm
} else:msvc {
    LIBS += -lRabbitIm0
}
include(pri/ThirdLibrary.pri)
include(pri/ThirdLibraryJoin.pri)

SOURCES += main.cpp

#应用程序图标  
RC_FILE = AppIcon.rc

CONFIG += localize_deployment  #本地语言部署  

#安装前缀  
isEmpty(PREFIX) {
    android {
        PREFIX = /.
    } else {
        PREFIX = $$OUT_PWD/install
    } 
}
other.files = License.md Authors.txt ChangeLog.md
other.path = $$PREFIX
other.CONFIG += directory no_check_exist 
target.path = $$PREFIX
!android : INSTALLS += other target

#ANDROID 平台相关内容  
android : include(android/android.pri)

win32 : equals(QMAKE_HOST.os, Windows){
    
    INSTALL_TARGET = $$system_path($${PREFIX}/$(TARGET)) #$(TARGET)是qmake产生脚本中的引用
    
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

    #安装第三方依赖库  
    Deployment_third_lib.target = Deployment_third_lib
    Deployment_third_lib.files = $$system_path($${THIRD_LIBRARY_PATH}/lib/*.dll)
    Deployment_third_lib.path = $$system_path($$PREFIX)
    Deployment_third_lib.CONFIG += directory no_check_exist

    Deployment_third_bin.target = Deployment_third_bin
    Deployment_third_bin.files = $$system_path($${THIRD_LIBRARY_PATH}/bin/*.dll)
    Deployment_third_bin.path = $$system_path($$PREFIX)
    Deployment_third_bin.CONFIG += directory no_check_exist
    INSTALLS += Deployment_qtlib Deployment_third_lib Deployment_third_bin
    #QMAKE_EXTRA_TARGETS += Deployment_qtlib Deployment_third_lib Deployment_third_bin
    
    #为调试环境复制动态库  
    !exists($${TARGET_PATH}/platforms/*){

        #复制QT系统插件  
        PlatformsPlugins.commands = \
            $(COPY_DIR) $$system_path($$[QT_INSTALL_PLUGINS]/platforms) $$system_path($${TARGET_PATH}/platforms)
        PlatformsPlugins.CONFIG += directory no_link no_clean no_check_exist
        PlatformsPlugins.target = PlatformsPlugins
        QMAKE_EXTRA_TARGETS += PlatformsPlugins
        COPY_THIRD_DEPENDS.depends = PlatformsPlugins

        #复制第三方依赖库动态库到编译输出目录  
        THIRD_LIBRARY_DLL = $${THIRD_LIBRARY_PATH}/bin/*.dll
        exists($${THIRD_LIBRARY_DLL}){
            equals(QMAKE_HOST.os, Windows) : msvc | isEmpty(QMAKE_SH){
                THIRD_LIBRARY_DLL = $$system_path($$THIRD_LIBRARY_DLL)
                TARGET_PATH = $$system_path($$TARGET_PATH)
            }
            ThirdLibraryDll.commands = \
                $${QMAKE_COPY} $${THIRD_LIBRARY_DLL} $${TARGET_PATH}
            ThirdLibraryDll.CONFIG += directory no_link no_clean no_check_exist
            ThirdLibraryDll.target = ThirdLibraryDll
            QMAKE_EXTRA_TARGETS += ThirdLibraryDll
            COPY_THIRD_DEPENDS.depends += ThirdLibraryDll
        }
    
        THIRD_LIBRARY_LIB = $${THIRD_LIBRARY_PATH}/lib/*.dll
        exists($${THIRD_LIBRARY_LIB}){
            equals(QMAKE_HOST.os, Windows) : msvc | isEmpty(QMAKE_SH){
                THIRD_LIBRARY_LIB = $$system_path($$THIRD_LIBRARY_LIB)
                TARGET_PATH = $$system_path($$TARGET_PATH)
            }
            ThirdLibraryLib.commands = \
                $${QMAKE_COPY} $${THIRD_LIBRARY_LIB} $${TARGET_PATH}
            ThirdLibraryLib.CONFIG += directory no_link no_clean no_check_exist
            ThirdLibraryLib.target = ThirdLibraryLib
            QMAKE_EXTRA_TARGETS += ThirdLibraryLib
            COPY_THIRD_DEPENDS.depends += ThirdLibraryLib
        }

        COPY_THIRD_DEPENDS.target = COPY_THIRD_DEPENDS
        COPY_THIRD_DEPENDS.commands = @echo copy third depends
        QMAKE_EXTRA_TARGETS += COPY_THIRD_DEPENDS
        POST_TARGETDEPS += COPY_THIRD_DEPENDS  #调试只要手动执行一次此目标  

    }
}
