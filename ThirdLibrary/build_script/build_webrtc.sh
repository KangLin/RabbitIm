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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/webrtc
fi

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    mkdir ${RABBITIM_BUILD_SOURCE_CODE}
    cd ${RABBITIM_BUILD_SOURCE_CODE}
    #下载 depot tools
    git clone -q https://chromium.googlesource.com/chromium/tools/depot_tools.git
    export PATH=${RABBITIM_BUILD_SOURCE_CODE}/depot_tools:$PATH
    VERSION=r8464
    gclient config --name src https://chromium.googlesource.com/external/webrtc 
    if [ "$1" = "windows_msvc" -o "$1" = "windows_mingw" ]; then
        echo "target_os = ['windows']" >> .gclient
    else
        echo "target_os = ['$1']" >> .gclient
    fi
    export DEPOT_TOOLS_WIN_TOOLCHAIN=0
    gclient sync --force
elif [ -d "${RABBITIM_BUILD_SOURCE_CODE}/depot_tools" ]; then
    export PATH=${RABBITIM_BUILD_SOURCE_CODE}/depot_tools:$PATH
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
sed -i "s/self.target.binary, 'alink_thin', link_deps/self.target.binary, 'alink', link_deps/g" ${RABBITIM_BUILD_SOURCE_CODE}/chromium/src/tools/gyp/pylib/gyp/generator/ninja.py

TARGET=all
case ${RABBITIM_BUILD_TARGERT} in
    android)
        ./build/install-build-deps-android.sh # --no-syms  --no-arm  --no-chromeos-fonts  --no-nacl #安装信赖 
        source build/android/envsetup.sh
        python webrtc/build/gyp_webrtc.py -D "clang=0" -D "OS=android"  -D linux_use_gold_flags=0 
        ;;
    unix)
        ./build/install-build-deps.sh --no-syms  --no-arm  --no-chromeos-fonts  --no-nacl #安装信赖 
        export GYP_GENERATORS=ninja 
        python webrtc/build/gyp_webrtc.py -D "clang=0" -D OS=linux  -D linux_use_gold_flags=0  -Dlinux_use_bundled_gold=0 # -D "linux_fpic=0" 
        TARGET=peerconnection_client
        ;;
    windows_msvc)
        export GYP_GENERATORS=ninja  
        export DEPOT_TOOLS_WIN_TOOLCHAIN=0
        python webrtc/build/gyp_webrtc.py   
        TARGET=peerconnection_client
        ;;
    windows_mingw)
        
         ;;
    *)
        echo "${HELP_STRING}"
        cd $CUR_DIR
        exit 2
        ;;
esac

ninja -C out/Debug $TARGET
ninja -C out/Release $TARGET

#生成头文件
find talk webrtc -name "*.h" > files.txt
tar czf  include.tar.gz --files-from files.txt
if [ ! -d "${RABBITIM_BUILD_PREFIX}/include" ]; then
    mkdir -p ${RABBITIM_BUILD_PREFIX}/include
fi
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
    android)
        cp `find out/Debug -path "obj.host" -prune -o -name "*.a"` ${RABBITIM_BUILD_PREFIX}/lib/Debug
        cp `find out/Release -path "obj.host" -prune -o -name "*.a"` ${RABBITIM_BUILD_PREFIX}/lib/Release
        ;;
    *)
    ;;
esac

cd $CUR_DIR
