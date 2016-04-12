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

echo ". `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh"
. `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh

if [ -n "$2" ]; then
    RABBITIM_BUILD_SOURCE_CODE=$2
else
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/webrtc
fi

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    VERSION=r8464
fi

CUR_DIR=`pwd`
RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_SOURCE_CODE}/src
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
    rm -fr out
fi

python setup_links.py --force

echo "configure ..."
if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
    CONFIG_PARA="--enable-static"
else
    CONFIG_PARA="--enable-shared"
fi
case ${RABBITIM_BUILD_TARGERT} in
    android)
        ./build/install-build-deps-android.sh --no-syms  --no-arm  --no-chromeos-fonts  --no-nacl #安装信赖 
        python webrtc/build/gyp_webrtc.py -D "clang=0" 
        ;;
    unix)
        ./build/install-build-deps.sh --no-syms  --no-arm  --no-chromeos-fonts  --no-nacl #安装信赖 
        python webrtc/build/gyp_webrtc.py -D "clang=0"   # -D "linux_fpic=0" 
        ;;
    windows_msvc)
        export GYP_GENERATORS=ninja # msvs or msvs-ninja   
        export DEPOT_TOOLS_WIN_TOOLCHAIN=0
        python webrtc/build/gyp_webrtc.py   
        ;;
    windows_mingw)
         ;;
    *)
        echo "${HELP_STRING}"
        cd $CUR_DIR
        exit 2
        ;;
esac

ninja -C out/Debug peerconnection_client
ninja -C out/Release peerconnection_client

#生成头文件
find talk webrtc -name "*.h" > files.txt
tar czf  include.tar.gz --files-from files.txt
tar xzf include.tar.gz -C ${RABBITIM_BUILD_PREFIX}/include
rm include.tar.gz files.txt

#复制库文件
if [ ! -d "${RABBITIM_BUILD_PREFIX}/lib/Debug" ]; then
    mkdir -p ${RABBITIM_BUILD_PREFIX}/lib/Debug
fi
if [ ! -d "${RABBITIM_BUILD_PREFIX}/lib/Release" ]; then
    mkdir ${RABBITIM_BUILD_PREFIX}/lib/Release
fi
case ${RABBITIM_BUILD_TARGERT} in
    windows_msvc)
        cp `find out/Debug -name "*.lib"` ${RABBITIM_BUILD_PREFIX}/lib/Debug
        cp `find out/Release -name "*.lib"` ${RABBITIM_BUILD_PREFIX}/lib/Release
    ;;
    unix)
        cp `find out/Debug -name "*.a"` ${RABBITIM_BUILD_PREFIX}/lib/Debug
        cp `find out/Release -name "*.a"` ${RABBITIM_BUILD_PREFIX}/lib/Release
        ;;
    *)
    ;;
esac

cd $CUR_DIR
