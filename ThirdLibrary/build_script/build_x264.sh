#!/bin/bash

#作者：康林
#参数:
#    $1:编译目标(android、windows_msvc、windows_mingw、unix)
#    $2:源码的位置 

#运行本脚本前,先运行 build_$1_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix）
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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/x264
fi

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    X264_VERSION=stable
    echo "git clone -q git://git.videolan.org/x264.git ${RABBITIM_BUILD_SOURCE_CODE}"
    #git clone -q git://git.videolan.org/x264.git ${RABBITIM_BUILD_SOURCE_CODE}
    git clone -q -b ${X264_VERSION} git://git.videolan.org/x264.git ${RABBITIM_BUILD_SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${RABBITIM_BUILD_SOURCE_CODE}

echo ""
echo "RABBITIM_BUILD_TARGERT:${RABBITIM_BUILD_TARGERT}"
echo "RABBITIM_BUILD_SOURCE_CODE:$RABBITIM_BUILD_SOURCE_CODE"
echo "CUR_DIR:`pwd`"
echo "RABBITIM_BUILD_PREFIX:$RABBITIM_BUILD_PREFIX"
echo "RABBITIM_BUILD_HOST:$RABBITIM_BUILD_HOST"
echo "RABBITIM_BUILD_CROSS_HOST:$RABBITIM_BUILD_CROSS_HOST"
echo "RABBITIM_BUILD_CROSS_PREFIX:$RABBITIM_BUILD_CROSS_PREFIX"
echo "RABBITIM_BUILD_CROSS_SYSROOT:$RABBITIM_BUILD_CROSS_SYSROOT"
echo ""

if [ -n "$RABBITIM_CLEAN" ]; then
    if [ -d ".git" ]; then
        git clean -xdf
    else
        make distclean
    fi
fi

echo "configure ..."
if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
    CONFIG_PARA="--enable-static"
else
    CONFIG_PARA="--enable-shared"
fi
case ${RABBITIM_BUILD_TARGERT} in
    android)
    CONFIG_PARA="--cross-prefix=${RABBITIM_BUILD_CROSS_PREFIX} --enable-static --host=$RABBITIM_BUILD_CROSS_HOST"
    CONFIG_PARA="${CONFIG_PARA} --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
    CFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
    CPPFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
    ;;
    unix)
    ;;
    windows_msvc)
        export MSYSTEM=MINGW32
        export CC=cl
    ;;
    windows_mingw)
        case `uname -s` in
            Linux*|Unix*|CYGWIN*)
                CONFIG_PARA="${CONFIG_PARA} --cross-prefix=${RABBITIM_BUILD_CROSS_PREFIX} --host=$RABBITIM_BUILD_CROSS_HOST"
                ;;
            MSYS*)
                CONFIG_PARA="${CONFIG_PARA} --host=$RABBITIM_BUILD_CROSS_HOST"
                ;;
        *)
            ;;
        esac
        ;;
    *)
    echo "${HELP_STRING}"
    cd $CUR_DIR
    exit 2
    ;;
esac

CONFIG_PARA="${CONFIG_PARA} --prefix=$RABBITIM_BUILD_PREFIX --disable-cli --disable-opencl --enable-pic "

echo "./configure ${CONFIG_PARA} --extra-cflags=\"${CFLAGS}\" --extra-asflags=\"${ASFLAGS}\""
./configure ${CONFIG_PARA} --extra-cflags="${CFLAGS}" --extra-asflags="${ASFLAGS}"

echo "make install"
make ${RABBITIM_MAKE_JOB_PARA} && make install

cd $CUR_DIR
