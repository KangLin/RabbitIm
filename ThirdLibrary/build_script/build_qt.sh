#!/bin/bash

#作者：康林
#参数:
#    $1:编译目标(android、windows_msvc、windows_mingw、unix)
#    $2:源码的位置 

#运行本脚本前,先运行 build_$1_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix)
#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_SOURCE_CODE    #源码目录
#   RABBITIM_BUILD_CROSS_PREFIX   #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

set -e
HELP_STRING="Usage $0 PLATFORM (android|windows_msvc|windows_mingw|unix) [SOURCE_CODE_ROOT_DIRECTORY]"

case $1 in
    android|windows_msvc|windows_mingw|unix)
    RABBITIM_BUILD_TARGERT=$1
    ;;
    *)
    echo "${HELP_STRING}"
    exit 1
    ;;
esac

if [ -z "${RABBITIM_BUILD_PREFIX}" ]; then
    echo ". `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh"
    . `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh
fi

if [ -n "$2" ]; then
    RABBITIM_BUILD_SOURCE_CODE=$2
else
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/qt5
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    QT_VERSION_DIR=5.6
    QT_VERSION=5.6.0
    mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
    cd ${RABBITIM_BUILD_SOURCE_CODE}
    if [ "TRUE" = "${RABBITIM_USE_REPOSITORIES}" ]; then
        echo "git clone -q http://code.qt.io/qt/qt5.git ${RABBITIM_BUILD_SOURCE_CODE}"
        git clone -q  http://code.qt.io/qt/qt5.git ${RABBITIM_BUILD_SOURCE_CODE}
        git checkout ${QT_VERSION}
        perl init-repository -f --branch
    else
        #wget -q http://mirrors.ustc.edu.cn/qtproject/archive/qt/$QT_VERSION_DIR/${QT_VERSION}/single/qt-everywhere-opensource-src-${QT_VERSION}.tar.gz
        wget -q -c http://download.qt.io/official_releases/qt/$QT_VERSION_DIR/${QT_VERSION}/single/qt-everywhere-opensource-src-${QT_VERSION}.tar.gz
        tar xzf qt-everywhere-opensource-src-${QT_VERSION}.tar.gz
        mv qt-everywhere-opensource-src-${QT_VERSION} ..
        rm -fr *
        cd ..
        rm -fr ${RABBITIM_BUILD_SOURCE_CODE}
        mv -f qt-everywhere-opensource-src-${QT_VERSION} ${RABBITIM_BUILD_SOURCE_CODE}
    fi
fi

cd ${RABBITIM_BUILD_SOURCE_CODE}

#清理
if [ -n "$RABBITIM_CLEAN" ]; then
    if [ -d ".git" ]; then
        echo "clean ..."
        qtrepotools/bin/qt5_tool -c

        #git clean -xdf
        #git submodule foreach --recursive "git clean -dfx"
        #echo $1
        #for i in `ls $1`;
        #do
        #       if [ -d $1/${i} ]; then
        #               echo "$1/${i}"
        #                cd $1/${i}
        #               git clean -xdf
        #        fi
        #done
    else
        if [ -f Makefile ]; then
            make distclean
            rm -f Makefile
        fi
    fi
    rm -fr ${RABBITIM_BUILD_PREFIX}/qt
fi

echo "RABBITIM_BUILD_SOURCE_CODE:$RABBITIM_BUILD_SOURCE_CODE"
echo "CUR_DIR:`pwd`"
echo "RABBITIM_BUILD_PREFIX:$RABBITIM_BUILD_PREFIX"
echo "RABBITIM_BUILD_HOST:$RABBITIM_BUILD_HOST"
echo "RABBITIM_BUILD_CROSS_HOST:$RABBITIM_BUILD_CROSS_HOST"
echo "RABBITIM_BUILD_CROSS_PREFIX:$RABBITIM_BUILD_CROSS_PREFIX"
echo "RABBITIM_BUILD_CROSS_SYSROOT:$RABBITIM_BUILD_CROSS_SYSROOT"
echo "RABBITIM_BUILD_STATIC:$RABBITIM_BUILD_STATIC"
echo ""

echo "configure ..."
CONFIG_PARA="-opensource -confirm-license -nomake examples -nomake tests -no-compile-examples"
CONFIG_PARA="${CONFIG_PARA} -no-sql-sqlite -no-sql-odbc -qt-xcb"
CONFIG_PARA="${CONFIG_PARA} -skip qtdoc -no-warnings-are-errors"
CONFIG_PARA="${CONFIG_PARA} -prefix ${RABBITIM_BUILD_PREFIX}/qt"
CONFIG_PARA="${CONFIG_PARA} -I ${RABBITIM_BUILD_PREFIX}/include -L ${RABBITIM_BUILD_PREFIX}/lib"
#CONFIG_PARA="${CONFIG_PARA} -developer-build  -debug-and-release"
if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
    CONFIG_PARA="${CONFIG_PARA} -static"
    #需要加入到qtbase\mkspecs\win32-g++\qmake.conf中
    QMAKE_LFLAGS="${QMAKE_LFLAGS} -static -static-libgcc"
else
    CONFIG_PARA="${CONFIG_PARA} -shared"
fi

if [ -d qt3d ]; then
    CONFIG_PARA="${CONFIG_PARA} -skip qt3d";
fi
if [ -d qtcanvas3d ]; then
    CONFIG_PARA="${CONFIG_PARA} -skip qtcanvas3d";
fi
if [ -d qtserialport ]; then
    CONFIG_PARA="${CONFIG_PARA} -skip qtserialport"
fi
if [ -d qtenginio ]; then
    CONFIG_PARA="${CONFIG_PARA} -skip qtenginio"
fi
if [ -d qtqa ]; then
    CONFIG_PARA="${CONFIG_PARA} -skip qtqa"
fi
if [ -d qtscript ]; then
    CONFIG_PARA="${CONFIG_PARA} -skip qtscript"
fi
if [ -d qtwayland ]; then
    CONFIG_PARA="${CONFIG_PARA} -skip qtwayland"
fi
if [ -d qtconnectivity ]; then
    CONFIG_PARA="${CONFIG_PARA} -skip qtconnectivity"
fi
if [ -d qtgraphicaleffects ]; then
    CONFIG_PARA="${CONFIG_PARA} -skip qtgraphicaleffects"
fi
if [ -d qtimageformats ]; then
    CONFIG_PARA="${CONFIG_PARA} -skip qtimageformats"
fi
if [ -d qtwebkit-examples ]; then
    CONFIG_PARA="${CONFIG_PARA} -skip qtwebkit-examples"
fi
#if [ -d qtwebengine ]; then
#    CONFIG_PARA="${CONFIG_PARA} -skip qtwebengine"
#fi

CONFIGURE="./configure"
MAKE_PARA="${RABBITIM_MAKE_JOB_PARA}"
MAKE="make"
MODULE_PARA="qtwebkit"
case ${RABBITIM_BUILD_TARGERT} in
    android)
        #export PKG_CONFIG_SYSROOT_DIR=${RABBITIM_BUILD_CROSS_SYSROOT} #qt编译时需要
        #export PKG_CONFIG_LIBDIR=${RABBITIM_BUILD_PREFIX}/lib/pkgconfig
        #platform:本机工具链(configure工具会自动检测)；xplatform：目标机工具链
        #qt工具和库分为本机工具和目标机工具、库两部分
        #qmake、uic、rcc、lrelease、lupdate 均为本机工具，需要用本机工具链编译
        #库都是目标机的库，所以需要目标机的工具链
        TARGET_OS=`uname -s`
        case $TARGET_OS in
            MINGW* | CYGWIN* | MSYS*)
                #export PATH=${RABBITIM_BUILD_SOURCE_CODE}/gnuwin32/bin:${PATH}
                CONFIG_PARA="${CONFIG_PARA} -platform win32-g++"
                if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
                    sed "s/^QMAKE_LFLAGS *=.*/QMAKE_LFLAGS = -static/g" $RABBITIM_BUILD_SOURCE_CODE/qtbase/mkspecs/win32-g++/qmake.conf
                else
                    sed "s/^QMAKE_LFLAGS *=.*/QMAKE_LFLAGS =/g" $RABBITIM_BUILD_SOURCE_CODE/qtbase/mkspecs/win32-g++/qmake.conf
                fi
                ;;
            Linux* | Unix*)
                ;;
            *)
                echo "Don't support target:$TARGET_OS, Please see build_qt.sh"
                cd $CUR_DIR
                exit 2
                ;;
        esac
        CONFIG_PARA="${CONFIG_PARA} -xplatform android-g++" #交叉平台编译工具
        CONFIG_PARA="${CONFIG_PARA} -android-sdk ${ANDROID_SDK_ROOT} -android-ndk ${ANDROID_NDK_ROOT}"
        CONFIG_PARA="${CONFIG_PARA} -android-ndk-host ${RABBITIM_BUILD_HOST}"
        CONFIG_PARA="${CONFIG_PARA} -android-toolchain-version ${RABBITIM_BUILD_TOOLCHAIN_VERSION}"
        CONFIG_PARA="${CONFIG_PARA} -android-ndk-platform android-${RABBITIM_BUILD_PLATFORMS_VERSION}"
        MODULE_PARA="${MODULE_PARA} module-qtandroidextras"
        ;;
    unix)
        CONFIG_PARA="${CONFIG_PARA} -skip qtandroidextras -skip qtandroidextras -skip qtmacextras -skip qtwinextras"
        ;;
    windows_msvc)
        #export PATH=${RABBITIM_BUILD_SOURCE_CODE}/gnuwin32/bin:${PATH}
        CONFIGURE="./configure.bat"
        CONFIG_PARA="${CONFIG_PARA} -platform win32-msvc2013" #  -icu -opengl desktop"
        CONFIG_PARA="${CONFIG_PARA} -skip qtandroidextras -skip qtx11extras -skip qtmacextras"
        MAKE_PARA=""
        MAKE="nmake"
        ;;
    windows_mingw)
        #export PATH=${RABBITIM_BUILD_SOURCE_CODE}/gnuwin32/bin:${PATH}
        #platform:本机工具链(configure工具会自动检测)；xplatform：目标机工具链
        #qt工具和库分为本机工具和目标机工具、库两部分
        #qmake、uic、rcc、lrelease、lupdate 均为本机工具，需要用本机工具链编译
        #库都是目标机的库，所以需要目标机的工具链
        if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
            sed -i "s/^QMAKE_LFLAGS *=.*/QMAKE_LFLAGS = -static/g" $RABBITIM_BUILD_SOURCE_CODE/qtbase/mkspecs/win32-g++/qmake.conf
        else
            sed -i "s/^QMAKE_LFLAGS *=.*/QMAKE_LFLAGS =/g" $RABBITIM_BUILD_SOURCE_CODE/qtbase/mkspecs/win32-g++/qmake.conf
        fi
        case `uname -s` in
            MINGW*|MSYS*)
                CONFIG_PARA="${CONFIG_PARA} -platform win32-g++"
                #MAKE=mingw32-make.exe
                ;;
            CYGWIN*)
                CONFIG_PARA="${CONFIG_PARA} -platform  win32-g++"
                CONFIG_PARA="${CONFIG_PARA} -xplatform win32-g++ -device-option CROSS_COMPILE=${RABBITIM_BUILD_CROSS_PREFIX}"
                ;;
            Linux*|Unix*|*)
                #export PKG_CONFIG_SYSROOT_DIR=${RABBITIM_BUILD_PREFIX} #qt编译时需要
                #export PKG_CONFIG_LIBDIR=${RABBITIM_BUILD_PREFIX}/lib/pkgconfig
                CONFIG_PARA="${CONFIG_PARA} -xplatform win32-g++"
                CONFIG_PARA="${CONFIG_PARA} -device-option CROSS_COMPILE=${RABBITIM_BUILD_CROSS_PREFIX}"
                CONFIG_PARA="${CONFIG_PARA} -skip qtwebkit"
                ;;
        esac
        CONFIG_PARA="${CONFIG_PARA} -skip qtandroidextras -skip qtx11extras -skip qtmacextras  -no-rpath"
        ;;
    *)
        echo "${HELP_STRING}"
        cd $CUR_DIR
        exit 3
        ;;
esac

#显示编译详细信息
#if [ "${RABBITIM_BUILD_TARGERT}" != "windows_msvc" ]; then
#    CONFIG_PARA="${CONFIG_PARA} -verbose"
#fi

#export INCLUDE="$INCLUDE:${RABBITIM_BUILD_PREFIX}/include"
#export LIB="$LIB:${RABBITIM_BUILD_PREFIX}/lib"

echo "$CONFIGURE ${CONFIG_PARA}"
$CONFIGURE ${CONFIG_PARA}

for PARA_VER in ${MODULE_PARA}
do
    INSTALL_MODULE_PARA="${INSTALL_MODULE_PARA} module-${PARA_VER}-install_subtargets"
done

echo "$MAKE ${MAKE_PARA} install"
#if [ "${RABBITIM_BUILD_TARGERT}" = "android" ]; then
#    $MAKE ${MAKE_PARA} \
#        &&  $MAKE install 
        
#else
    $MAKE ${MAKE_PARA} \
        &&  $MAKE install 
#fi

cat > ${RABBITIM_BUILD_PREFIX}/qt/bin/qt.conf << EOF
[Paths]
Prefix=..
EOF


cd $CUR_DIR
