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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/libqrencode
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    VERSION=3.4.3
    #if [ "TRUE" = "${RABBITIM_USE_REPOSITORIES}" ]; then
        echo "git clone -q --branch=v$VERSION https://github.com/fukuchi/libqrencode.git ${RABBITIM_BUILD_SOURCE_CODE}"
        git clone -q  https://github.com/fukuchi/libqrencode.git ${RABBITIM_BUILD_SOURCE_CODE}
    #else
    #    echo "wget -q https://github.com/fukuchi/libqrencode/archive/v${VERSION}.tar.gz"
    #    mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
    #    cd ${RABBITIM_BUILD_SOURCE_CODE}
    #    wget -q -c http://fukuchi.org/works/qrencode/qrencode-3.4.4.tar.gz
    #    tar xf qrencode-3.4.4.tar.gz
    #    mv qrencode-3.4.4  ..
    #    rm -fr *
    #    cd ..
    #    rm -fr ${RABBITIM_BUILD_SOURCE_CODE}
    #    mv -f qrencode-3.4.4 ${RABBITIM_BUILD_SOURCE_CODE}
    #fi
fi

cd ${RABBITIM_BUILD_SOURCE_CODE}

if [ "$RABBITIM_CLEAN" ]; then
    if [ -d ".git" ]; then
        echo "git clean -xdf"
        git clean -xdf
    fi
fi

if [ ! -f configure ]; then
    mkdir -p m4
    echo "sh autogen.sh"
    sh autogen.sh
fi

mkdir -p build_${RABBITIM_BUILD_TARGERT}
cd build_${RABBITIM_BUILD_TARGERT}
if [ -n "$RABBITIM_CLEAN" ]; then
    rm -fr *
fi

echo ""
echo "RABBITIM_BUILD_TARGERT:${RABBITIM_BUILD_TARGERT}"
echo "RABBITIM_BUILD_SOURCE_CODE:$RABBITIM_BUILD_SOURCE_CODE"
echo "CUR_DIR:`pwd`"
echo "RABBITIM_BUILD_PREFIX:$RABBITIM_BUILD_PREFIX"
echo "RABBITIM_BUILD_HOST:$RABBITIM_BUILD_HOST"
echo "RABBITIM_BUILD_CROSS_HOST:$RABBITIM_BUILD_CROSS_HOST"
echo "RABBITIM_BUILD_CROSS_PREFIX:$RABBITIM_BUILD_CROSS_PREFIX"
echo "RABBITIM_BUILD_CROSS_SYSROOT:$RABBITIM_BUILD_CROSS_SYSROOT"
echo "RABBITIM_BUILD_STATIC:$RABBITIM_BUILD_STATIC"
echo "PKG_CONFIG_PATH:${PKG_CONFIG_PATH}"
echo "PKG_CONFIG_LIBDIR:${PKG_CONFIG_LIBDIR}"
echo "PATH:${PATH}"
echo ""

echo "configure ..."
MAKE=make

if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
    CONFIG_PARA="--enable-static --disable-shared"
else
    CONFIG_PARA="--disable-static --enable-shared"
fi
case ${RABBITIM_BUILD_TARGERT} in
    android)
        export CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc 
        export CXX=${RABBITIM_BUILD_CROSS_PREFIX}g++
        export AR=${RABBITIM_BUILD_CROSS_PREFIX}ar
        export LD=${RABBITIM_BUILD_CROSS_PREFIX}ld
        export AS=${RABBITIM_BUILD_CROSS_PREFIX}as
        export STRIP=${RABBITIM_BUILD_CROSS_PREFIX}strip
        export NM=${RABBITIM_BUILD_CROSS_PREFIX}nm
        CONFIG_PARA="CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc LD=${RABBITIM_BUILD_CROSS_PREFIX}ld"
        CONFIG_PARA="${CONFIG_PARA} --disable-shared -enable-static --host=$RABBITIM_BUILD_CROSS_HOST"
        CONFIG_PARA="${CONFIG_PARA} --with-sysroot=${RABBITIM_BUILD_PREFIX}"
        CFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        CPPFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        ;;
    unix)
        ;;
    windows_msvc)
        #https://folti.blogs.balabit.com/2009/08/compiling-autoconfmake-projects-under-msvc-part-one/
        #../configure CXX="cl -nologo" CC="cl -nologo" CFLAGS=-MD \
        #    LD=link NM="dumpbin -symbols" STRIP=: RANLIB=: \
        #    --enable-dependency-tracking 
            
        echo "Qrencode don't complie in windows_msvc "
        cd $CUR_DIR
        exit 0
        ;;
    windows_mingw)
        CONFIG_PARA="${CONFIG_PARA} CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc --host=${RABBITIM_BUILD_CROSS_HOST} "
        CONFIG_PARA="${CONFIG_PARA} --with-gnu-ld"
        ;;
    *)
        echo "${HELP_STRING}"
        cd $CUR_DIR
        exit 3
        ;;
esac

echo "make install"
echo "pwd:`pwd`"
CONFIG_PARA="${CONFIG_PARA} --prefix=${RABBITIM_BUILD_PREFIX} "
CONFIG_PARA="${CONFIG_PARA} --without-tools" # --without-png --without-sdl"

if [ "${RABBITIM_BUILD_TARGERT}" = android ]; then
    echo "../configure ${CONFIG_PARA} CFLAGS=\"${CFLAGS=}\" CPPFLAGS=\"${CPPFLAGS}\""
    CFLAGS="-mthumb" CXXFLAGS="-mthumb" ../configure ${CONFIG_PARA} CFLAGS="${CFLAGS}" CPPFLAGS="${CPPFLAGS}"
else
    echo "../configure ${CONFIG_PARA}"
    ../configure ${CONFIG_PARA}
fi

${MAKE} && ${MAKE} install

if [ "${RABBITIM_BUILD_TARGERT}" = "windows_msvc" ]; then
    cd ${RABBITIM_BUILD_PREFIX}/lib
    mv libqrencode.a qrencode.lib
fi

cd $CUR_DIR
