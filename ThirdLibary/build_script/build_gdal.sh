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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/gdal
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    GDAL_VERSION
    if [ "TRUE" = "$RABBITIM_USE_REPOSITORIES" ]; then
        echo "git clone --branch=${GDAL_VERSION} https://github.com/OSGeo/gdal ${RABBITIM_BUILD_SOURCE_CODE}"
        git clone --branch=$GDAL_VERSION https://github.com/OSGeo/gdal ${RABBITIM_BUILD_SOURCE_CODE}
    else
        echo "wget https://github.com/OSGeo/gdal/archive/${GDAL_VERSION}.zip"
        mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        wget wget https://github.com/OSGeo/gdal/archive/${GDAL_VERSION}.zip
        unzip ${GDAL_VERSION}.zip
        mv gdal-${GDAL_VERSION} ..
        rm -fr *
        cd ..
        mv -f gdal-${GDAL_VERSION} ${RABBITIM_BUILD_SOURCE_CODE}
    fi
fi

RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_SOURCE_CODE}/gdal
cd ${RABBITIM_BUILD_SOURCE_CODE}

if [ "$RABBITIM_CLEAN" ]; then
    if [ -d "../.git" ]; then
        echo "git clean -xdf"
        git clean -xdf
    else
        make clean
    fi
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
        CONFIG_PARA="CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc --disable-shared -enable-static"
        CONFIG_PARA="$CONFIG_PARA --host=${RABBITIM_BUILD_CROSS_HOST} --with-sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        CFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        CPPFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        ;;
    unix)
        ;;
    windows_msvc)
        ;;
    windows_mingw)
        case `uname -s` in
            Linux*|Unix*|CYGWIN*)
                CONFIG_PARA="${CONFIG_PARA} CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc --host=${RABBITIM_BUILD_CROSS_HOST} "
                CONFIG_PARA="${CONFIG_PARA} --with-gnu-ld"
                ;;
            MINGW* | MSYS*)
                ;;
            *)
            ;;
        esac
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

if [ "${RABBITIM_BUILD_TARGERT}" = android ]; then
    echo "./configure ${CONFIG_PARA} CFLAGS=\"${CFLAGS=}\" CPPFLAGS=\"${CPPFLAGS}\""
    ./configure ${CONFIG_PARA} CFLAGS="${CFLAGS}" CPPFLAGS="${CPPFLAGS}"
else
    echo "./configure ${CONFIG_PARA}"
    ./configure ${CONFIG_PARA}
fi

${MAKE} ${RABBITIM_MAKE_JOB_PARA} && ${MAKE} install

cd $CUR_DIR
