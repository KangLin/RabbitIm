# 注意：Qt 版本必须大于 5.0  
lessThan(QT_MAJOR_VERSION, 5) : error("version is $$QT_VERSION, please qt is used greater then 5.0")

QT *= core gui network xml multimedia widgets

qtHaveModule(webkit) {
    QT *= webkitwidgets
    DEFINES *= RABBITIM_WEBKIT 
}

DEFINES *= RABBITIM
CONFIG *= c++11
msvc{
    QMAKE_CXXFLAGS *= "/MP"
}

CONFIG(debug, debug|release)  {
    #调试宏   
    DEFINES *= DEBUG #DEBUG_VIDEO_TIME  
}

#android选项中包含了unix选项，所以在写工程如下条件判断时，必须把android条件放在unix条件前  
win32 {
    DEFINES *= WINDOWS
    RABBITIM_SYSTEM = "windows"

    contains(QMAKE_TARGET.arch, x86_64){
        RABBITIM_ARCHITECTURE = "x64"
    }else {
        RABBITIM_ARCHITECTURE = "x86"
    }

    RABBIT_TOOLCHAIN_VERSION=$$(RABBIT_TOOLCHAIN_VERSION)
    msvc {
        QMAKE_CXXFLAGS *= /wd"4819 /utf-8"  #忽略msvc下对utf-8的警告,支持UTF-8编码  
        #QMAKE_LFLAGS += -ladvapi32
        
        RABBITIM_PLATFORM = "windows_msvc"
        isEmpty(RABBIT_TOOLCHAIN_VERSION) {    
            VisualStudioVersion = $$(VisualStudioVersion)
            contains(VisualStudioVersion, 15.0):RABBIT_TOOLCHAIN_VERSION = "15"
            contains(VisualStudioVersion, 14.0):RABBIT_TOOLCHAIN_VERSION = "14"
            contains(VisualStudioVersion, 12.0):RABBIT_TOOLCHAIN_VERSION = "12"
        }
        debug {
            QMAKE_LFLAGS *= /SUBSYSTEM:WINDOWS",5.01" /NODEFAULTLIB:libcmtd
        }else{
            QMAKE_LFLAGS *= /SUBSYSTEM:WINDOWS",5.01" /NODEFAULTLIB:libcmt
        }
    } else {
        RABBITIM_PLATFORM = "windows_mingw"
        #TODO:根据实际情况修改  
        isEmpty(RABBIT_TOOLCHAIN_VERSION) {
            greaterThan(QT_MAJOR_VERSION, 4) : greaterThan(QT_MINOR_VERSION, 9) {
                RABBIT_TOOLCHAIN_VERSION=530
            } else {
                RABBIT_TOOLCHAIN_VERSION=492
            }
        }
        DEFINES *= "_WIN32_WINNT=0x0501" #__USE_MINGW_ANSI_STDIO
    }

} else:unix {
    message("unix: QMAKE_TARGET.arch:$$QMAKE_TARGET.arch")
    RABBITIM_SYSTEM = unix
    RABBITIM_PLATFORM = unix
    DEFINES *= UNIX

    contains(QMAKE_TARGET.arch, x86_64){
        RABBITIM_ARCHITECTURE = "x64"
    }else {
        RABBITIM_ARCHITECTURE = "x86"
    }
    #TODO:
    RABBITIM_ARCHITECTURE = "x64"
} else:android {
    message("android: QMAKE_TARGET.arch:$$QMAKE_TARGET.arch")
    RABBITIM_SYSTEM = "android"
    RABBITIM_PLATFORM = "android"
    RABBITIM_ARCHITECTURE = $${ANDROID_ARCHITECTURE}
    API=$$(ANDROID_NDK_PLATFORM)
    RABBIT_TOOLCHAIN_VERSION=$$split(API, "android-")
    DEFINES *= ANDROID MOBILE
}

isEmpty(RABBIT_CONFIG) {
    CONFIG(debug, debug|release) {
        RABBIT_CONFIG=Debug
    } else {
        RABBIT_CONFIG=Release
    }
}

isEmpty(THIRD_LIBRARY_PATH) : THIRD_LIBRARY_PATH = $$(THIRD_LIBRARY_PATH)
isEmpty(THIRD_LIBRARY_PATH) : THIRD_LIBRARY_PATH = $$PWD/../ThirdLibrary/$${RABBITIM_PLATFORM}$${RABBIT_TOOLCHAIN_VERSION}_$${RABBITIM_ARCHITECTURE}_$${RABBIT_CONFIG}_qt$${QT_VERSION}

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
}else{
    DEFINES *= BUILD_SHARED_LIBS #windows下动态库
}
message("THIRD_LIBRARY_PATH=$${THIRD_LIBRARY_PATH}")
!exists($$THIRD_LIBRARY_PATH) : warning("Please set THIRD_LIBRARY_PATH")

INCLUDEPATH *= $$PWD/..
INCLUDEPATH *= $${THIRD_LIBRARY_PATH}/include
#DEPENDPATH *= $${THIRD_LIBRARY_PATH}/include
LIBS *= -L$${THIRD_LIBRARY_PATH}/lib
android : LIBS *= -L$${THIRD_LIBRARY_PATH}/libs/$${ANDROID_TARGET_ARCH}
LIBS *= $$LDFLAGS
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
    PKG_CONFIG_PATH="$${THIRD_LIBRARY_PATH}/lib/pkgconfig:$${TARGET_PATH}/pkgconfig:$$(PKG_CONFIG_PATH)"
} else {
    PKG_CONFIG_PATH="$${THIRD_LIBRARY_PATH}/lib/pkgconfig:$${TARGET_PATH}/pkgconfig:$$(PKG_CONFIG_PATH)"
}

isEmpty(PKG_CONFIG) : PKG_CONFIG=$$(PKG_CONFIG)
isEmpty(PKG_CONFIG) : PKG_CONFIG = pkg-config

CONFIG(static, static|shared) {
    PKG_CONFIG *= --static
}

sysroot.name = PKG_CONFIG_SYSROOT_DIR
sysroot.value = $$PKG_CONFIG_SYSROOT_DIR
libdir.name = PKG_CONFIG_LIBDIR
libdir.value = $$PKG_CONFIG_LIBDIR
path.name = PKG_CONFIG_PATH
path.value = $$PKG_CONFIG_PATH
qtAddToolEnv(PKG_CONFIG,libdir path, SYS)
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
            !msvc : message("Warring: package $$package is not exist. ")
            mingw | equals(QMAKE_HOST.os, Windows) : message("Warring: package $$package is not exist. Be sure use pkg-config in mingw32?")
            return(false)
        }
    }

    return(true)
}
