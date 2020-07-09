#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T21:47:50
#
#-------------------------------------------------

TARGET = RabbitImApp
TEMPLATE = app 

#设置目标输出目录  
isEmpty(DESTDIR): DESTDIR = $$OUT_PWD/../bin

INCLUDEPATH *= ../Src
win32: LIBS *= -L$$DESTDIR
else: LIBS *= $$OUT_PWD/../lib
LIBS += -lRabbitIm

include(../pri/ThirdLibraryConfig.pri)
CONFIG(static, static|shared) {
    #CONFIG *= link_prl
    include(../Plugin/PluginStatic.pri)
} else {
    DEFINES += BUILD_SHARED_LIBS #windows下动态库
}

include(../pri/ThirdLibrary.pri)
include(../pri/ThirdLibraryJoin.pri)

isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$(RabbitCommon_DIR)
isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$PWD/../../RabbitCommon
!isEmpty(RabbitCommon_DIR): exists("$${RabbitCommon_DIR}/Src/Src.pro"){
    include($${RabbitCommon_DIR}/pri/Translations.pri)
    DEFINES *= RABBITCOMMON
    INCLUDEPATH *= $${RabbitCommon_DIR}/Src $${RabbitCommon_DIR}/Src/export
    LIBS += -lRabbitCommon
} else{
    message("Don't find RabbitCommon, in environment variable RabbitCommon_DIR:$$RabbitCommon_DIR")
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon ag:")
    message("   git clone https://github.com/KangLin/RabbitCommon.git")
    error  ("2. Then set environment variable RabbitCommon_DIR to download dirctory")
}

SOURCES += main.cpp

CONFIG += localize_deployment  #本地语言部署  

#安装前缀
isEmpty(PREFIX) : !isEmpty(INSTALL_ROOT) : PREFIX=$$INSTALL_ROOT
isEmpty(PREFIX) {
    qnx : PREFIX = /tmp
    else : android : PREFIX = /.
    else : PREFIX = $$OUT_PWD/../install
}
other.files = ../License.md ../Authors.txt ../ChangeLog.md
android: other.path = $$PREFIX/assets
else: other.path = $$PREFIX
other.CONFIG += directory no_check_exist 
target.path = $$PREFIX/bin
win32{
    #应用程序图标  
    RC_FILE = AppIcon.rc

    icon.files = ../Resource/png/RabbitIm.ico
    icon.path = $$PREFIX
    icon.CONFIG += directory no_check_exist

    install.files = ../Install/Install.nsi
    install.path = $$OUT_PWD/..
    install.CONFIG += directory no_check_exist
    INSTALLS += install icon
}
INSTALLS += other target
!android : unix {
    DESKTOP_FILE.target = DESKTOP_FILE
    DESKTOP_FILE.files = ../share/applications/RabbitIm.desktop
    DESKTOP_FILE.path = $$system_path($${PREFIX})/share/applications
    DESKTOP_FILE.CONFIG += directory no_check_exist

    # install icons
    icon128.target = icon128
    icon128.files = ../Resource/png/RabbitIm.png
    icon128.path = $${PREFIX}/share/pixmaps
    icon128.CONFIG = directory no_check_exist

    INSTALLS += DESKTOP_FILE icon128
}

#ANDROID 平台相关内容  
android : include(../android/android.pri)

win32 : equals(QMAKE_HOST.os, Windows){
    
    INSTALL_TARGET = $$system_path($${DESTDIR}/$(TARGET)) #$(TARGET)是qmake产生脚本中的引用
    
    #mingw{  #手机平台不需要  
    #    RABBITIM_STRIP.target = RABBITIM_STRIP
    #    RABBITIM_STRIP.commands = "strip $$INSTALL_TARGET"
    #    INSTALLS += RABBITIM_STRIP
    #}
    #安装qt依赖库  
    Deployment_qtlib.target = Deployment_qtlib
    Deployment_qtlib.files = $$system_path($${DESTDIR}/)  #把目录 DESTDIR 安装到下面 path 中,注意要加 / ,否则不包括目录，只复制目录中的文件
    Deployment_qtlib.path = $$system_path($${PREFIX})
    Deployment_qtlib.commands = "$$system_path($$[QT_INSTALL_BINS]/windeployqt)" \
                    --compiler-runtime \
                    --verbose 7 \
                    "$${INSTALL_TARGET}"

    #安装第三方依赖库  
    Deployment_third_lib.target = Deployment_third_lib
    Deployment_third_lib.files = $$system_path($${THIRD_LIBRARY_PATH}/lib/*.dll)
    Deployment_third_lib.path = $$system_path($$PREFIX/bin)
    Deployment_third_lib.CONFIG += directory no_check_exist

    Deployment_third_bin.target = Deployment_third_bin
    Deployment_third_bin.files = $$system_path($${THIRD_LIBRARY_PATH}/bin/*.dll)
    Deployment_third_bin.path = $$system_path($$PREFIX/bin)
    Deployment_third_bin.CONFIG += directory no_check_exist
    INSTALLS += Deployment_qtlib Deployment_third_lib Deployment_third_bin
    #QMAKE_EXTRA_TARGETS += Deployment_qtlib Deployment_third_lib Deployment_third_bin
    
    #为调试环境复制动态库  
    !exists($${DESTDIR}/platforms/*){

        #复制QT系统插件  
        PlatformsPlugins.commands = \
            $(COPY_DIR) $$system_path($$[QT_INSTALL_PLUGINS]/platforms) $$system_path($${DESTDIR}/platforms)
        PlatformsPlugins.CONFIG += directory no_link no_clean no_check_exist
        PlatformsPlugins.target = PlatformsPlugins
        QMAKE_EXTRA_TARGETS += PlatformsPlugins
        COPY_THIRD_DEPENDS.depends = PlatformsPlugins

        #复制第三方依赖库动态库到编译输出目录  
        THIRD_LIBRARY_DLL = $${THIRD_LIBRARY_PATH}/bin/*.dll
        exists($${THIRD_LIBRARY_DLL}){
            equals(QMAKE_HOST.os, Windows) : msvc | isEmpty(QMAKE_SH){
                THIRD_LIBRARY_DLL = $$system_path($$THIRD_LIBRARY_DLL)
                DESTDIR = $$system_path($$DESTDIR)
            }
            ThirdLibraryDll.commands = \
                $${QMAKE_COPY} $${THIRD_LIBRARY_DLL} $${DESTDIR}
            ThirdLibraryDll.CONFIG += directory no_link no_clean no_check_exist
            ThirdLibraryDll.target = ThirdLibraryDll
            QMAKE_EXTRA_TARGETS += ThirdLibraryDll
            COPY_THIRD_DEPENDS.depends += ThirdLibraryDll
        }
    
        THIRD_LIBRARY_LIB = $${THIRD_LIBRARY_PATH}/lib/*.dll
        exists($${THIRD_LIBRARY_LIB}){
            equals(QMAKE_HOST.os, Windows) : msvc | isEmpty(QMAKE_SH){
                THIRD_LIBRARY_LIB = $$system_path($$THIRD_LIBRARY_LIB)
                DESTDIR = $$system_path($$DESTDIR)
            }
            ThirdLibraryLib.commands = \
                $${QMAKE_COPY} $${THIRD_LIBRARY_LIB} $${DESTDIR}
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
