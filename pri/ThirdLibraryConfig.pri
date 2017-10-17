# 注意：Qt 版本必须大于 5.0  
lessThan(QT_VERSION, 5.0) : error("version is $$QT_VERSION, please qt is used greater then 5.0")

QT *= core gui network xml multimedia widgets

qtHaveModule(webkit) {
    QT *= webkitwidgets
    DEFINES *= RABBITIM_WEBKIT 
}

DEFINES *= RABBITIM
CONFIG *= c++11
msvc{
    QMAKE_CXXFLAGS *= " /MP "
}

CONFIG(debug, debug|release)  {
    #调试宏   
    DEFINES += DEBUG #DEBUG_VIDEO_TIME  
}

#android选项中包含了unix选项，所以在写工程如下条件判断时，必须把android条件放在unix条件前  
win32 {
    DEFINES += WINDOWS
    RABBITIM_SYSTEM = "windows"

    contains(QMAKE_TARGET.arch, x86_64){
        RABBITIM_ARCH = "x64"
    }else {
        RABBITIM_ARCH = "x86"
    }

    msvc {
        QMAKE_CXXFLAGS += /wd"4819"  #忽略msvc下对utf-8的警告  
        #QMAKE_LFLAGS += -ladvapi32
        RABBITIM_PLATFORM = "windows_msvc"

        debug {
            QMAKE_LFLAGS *= /SUBSYSTEM:WINDOWS",5.01" /NODEFAULTLIB:libcmtd
        }else{
            QMAKE_LFLAGS *= /SUBSYSTEM:WINDOWS",5.01" /NODEFAULTLIB:libcmt
        }
    } else {
        RABBITIM_PLATFORM = "windows_mingw"
        DEFINES += "_WIN32_WINNT=0x0501" #__USE_MINGW_ANSI_STDIO
    }

    isEmpty(THIRD_LIBRARY_PATH) : THIRD_LIBRARY_PATH = $$PWD/../ThirdLibrary/$${RABBITIM_PLATFORM}_$${RABBITIM_ARCH}

} else:android {
    message("QMAKE_TARGET.arch:$$QMAKE_TARGET.arch")
    RABBITIM_SYSTEM = "android"
    RABBITIM_ARCH = $${ANDROID_ARCHITECTURE}
    isEmpty(THIRD_LIBRARY_PATH) : THIRD_LIBRARY_PATH = $$PWD/../ThirdLibrary/android_$${ANDROID_ARCHITECTURE}
    DEFINES += ANDROID MOBILE
    
}  else:unix {

    RABBITIM_SYSTEM = unix
    DEFINES += UNIX
    isEmpty(THIRD_LIBRARY_PATH) : THIRD_LIBRARY_PATH = $$PWD/../ThirdLibrary/unix

}

CONFIG(static, static|shared) {
    DEFINES += RABBITIM_STATIC
    exists("$${THIRD_LIBRARY_PATH}_static"){
        THIRD_LIBRARY_PATH=$${THIRD_LIBRARY_PATH}_static
    }
    CONFIG *= link_prl static #生成静态程序  
   # CONFIG += staticlib # this is needed if you create a static library, not a static executable
   # DEFINES+= STATIC
#} else {
#    CONFIG += staticlib #生成静态库    
#    CONFIG += shared    #生成动态库  
}
message("THIRD_LIBRARY_PATH:$${THIRD_LIBRARY_PATH}")

INCLUDEPATH *= $$PWD/.. $$PWD/../common $$PWD/../Widgets/FrmCustom
INCLUDEPATH *= $${THIRD_LIBRARY_PATH}/include
#DEPENDPATH *= $${THIRD_LIBRARY_PATH}/include
LIBS *= -L$${THIRD_LIBRARY_PATH}/lib
android : LIBS += -L$${THIRD_LIBRARY_PATH}/libs/$${ANDROID_TARGET_ARCH}
LIBS += $$LDFLAGS
CONFIG(debug, debug|release) {
    LIBS *= -L$${THIRD_LIBRARY_PATH}/lib/Debug
} else {
    LIBS *= -L$${THIRD_LIBRARY_PATH}/lib/Release
}

#####以下配置 pkg-config  
mingw{
    equals(QMAKE_HOST.os, Windows){
        PKG_CONFIG_PATH="$${THIRD_LIBRARY_PATH}/lib/pkgconfig;$${TARGET_PATH}/pkgconfig;$$(PKG_CONFIG_PATH)"
    } else {
        PKG_CONFIG_PATH="$${THIRD_LIBRARY_PATH}/lib/pkgconfig:$${TARGET_PATH}/pkgconfig"
        PKG_CONFIG_SYSROOT_DIR=$${THIRD_LIBRARY_PATH}
    }
} else : msvc {
    PKG_CONFIG_SYSROOT_DIR=$${THIRD_LIBRARY_PATH}
    PKG_CONFIG_LIBDIR="$${THIRD_LIBRARY_PATH}/lib/pkgconfig;$${TARGET_PATH}/pkgconfig"
} else : android {
    PKG_CONFIG_SYSROOT_DIR=$${THIRD_LIBRARY_PATH} 
    equals(QMAKE_HOST.os, Windows){
        PKG_CONFIG_LIBDIR=$${THIRD_LIBRARY_PATH}/lib/pkgconfig;$${THIRD_LIBRARY_PATH}/libs/$${ANDROID_TARGET_ARCH}/pkgconfig;$${TARGET_PATH}/pkgconfig
    } else {
        PKG_CONFIG_LIBDIR=$${THIRD_LIBRARY_PATH}/lib/pkgconfig:$${THIRD_LIBRARY_PATH}/libs/$${ANDROID_TARGET_ARCH}/pkgconfig:$${TARGET_PATH}/pkgconfig
    }
} else {
    PKG_CONFIG_PATH="$${THIRD_LIBRARY_PATH}/lib/pkgconfig:$${TARGET_PATH}/pkgconfig:$$(PKG_CONFIG_PATH)"
}

isEmpty(PKG_CONFIG) : PKG_CONFIG=$$(PKG_CONFIG)

isEmpty(PKG_CONFIG) {
    PKG_CONFIG = pkg-config
}

CONFIG(static, static|shared) {
    PKG_CONFIG *= --static
}

sysroot.name = PKG_CONFIG_SYSROOT_DIR
sysroot.value = $$PKG_CONFIG_SYSROOT_DIR
libdir.name = PKG_CONFIG_LIBDIR
libdir.value = $$PKG_CONFIG_LIBDIR
path.name = PKG_CONFIG_PATH
path.value = $$PKG_CONFIG_PATH
qtAddToolEnv(PKG_CONFIG, sysroot libdir path, SYS)
equals(QMAKE_HOST.os, Windows): \
    PKG_CONFIG += 2> NUL
else: \
    PKG_CONFIG += 2> /dev/null

defineReplace(myPkgConfigExecutable) {
    return($$PKG_CONFIG)
}

defineTest(myPackagesExist) {
    pkg_config = $$myPkgConfigExecutable()

    for(package, ARGS) {
        !system($$pkg_config --exists $$package) {
            !msvc : message("Warring: package $$package is not exist.")
            return(false)
        }
    }

    return(true)
}
