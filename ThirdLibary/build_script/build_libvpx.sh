#!/bin/sh

#作者：康林
#参数:
#    $1:编译目标(android、windows_msvc、windows_mingw、unix、unix_mingw)
#    $2:源码的位置 

#运行本脚本前,先运行 build_$1_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix、unix_mingw）
#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_SOURCE_CODE    #源码目录
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

set -e
HELP_STRING="Usage $0 PLATFORM (android|windows_msvc|windows_mingw|unix|unix_mingw) [SOURCE_CODE_ROOT_DIRECTORY]"

case $1 in
    android|windows_msvc|windows_mingw|unix|unix_mingw)
    RABBITIM_BUILD_TARGERT=$1
    ;;
    *)
    echo "${HELP_STRING}"
    return 1
    ;;
esac

if [ -z "${RABBITIM_BUILD_PREFIX}" ]; then
    echo "source build_${RABBITIM_BUILD_TARGERT}_envsetup.sh"
    source build_${RABBITIM_BUILD_TARGERT}_envsetup.sh
fi

if [ -n "$2" ]; then
    RABBITIM_BUILD_SOURCE_CODE=$2
else
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/libvpx
fi

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    echo "git clone https://chromium.googlesource.com/webm/libvpx ${RABBITIM_BUILD_SOURCE_CODE}"
    git clone https://chromium.googlesource.com/webm/libvpx ${RABBITIM_BUILD_SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${RABBITIM_BUILD_SOURCE_CODE}

mkdir -p build_${RABBITIM_BUILD_TARGERT}
cd build_${RABBITIM_BUILD_TARGERT}
rm -fr *

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

echo "configure ..."
CONFIG_PARA="--enable-shared --disable-static"
case ${RABBITIM_BUILD_TARGERT} in
    android)
        CONFIG_PARA="--target=armv7-android-gcc --sdk-path=${ANDROID_NDK_ROOT} --disable-shared --enable-static"
        CFLAGS="-march=armv7-a -mfloat-abi=softfp -mfpu=neon "
    
        #编译 cpufeatures
        echo "${RABBITIM_BUILD_CROSS_PREFIX}gcc -I${RABBITIM_BUILD_CROSS_SYSROOT}/usr/include -c ${ANDROID_NDK_ROOT}/sources/android/cpufeatures/cpu-features.c"
        ${RABBITIM_BUILD_CROSS_PREFIX}gcc -I${RABBITIM_BUILD_CROSS_SYSROOT}/usr/include -c ${ANDROID_NDK_ROOT}/sources/android/cpufeatures/cpu-features.c
        ${RABBITIM_BUILD_CROSS_PREFIX}ar rcs  libcpu-features.a cpu-features.o
        cp libcpu-features.a ${RABBITIM_BUILD_PREFIX}/lib/.
		;;
    unix)
		;;
    windows_msvc)
		;;
    windows_mingw)
        CONFIG_PARA=""
		;;
	unix_mingw)
		export CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc 
		export CXX=${RABBITIM_BUILD_CROSS_PREFIX}g++
		export AR=${RABBITIM_BUILD_CROSS_PREFIX}ar
		export LD=${RABBITIM_BUILD_CROSS_PREFIX}gcc
		export AS=yasm
		export STRIP=${RABBITIM_BUILD_CROSS_PREFIX}strip
		export NM=${RABBITIM_BUILD_CROSS_PREFIX}nm
		CONFIG_PARA=" --target=x86-win32-gcc"
		;;
    *)
		echo "${HELP_STRING}"
		return 2
		;;
esac

CONFIG_PARA="${CONFIG_PARA}  --enable-libs --prefix=${RABBITIM_BUILD_PREFIX}  --disable-docs --disable-examples --disable-install-docs --disable-install-bins --enable-install-libs --disable-unit-tests --disable-debug --disable-debug-libs"

echo "./configure ${CONFIG_PARA} --extra-cflags=\"${CFLAGS=}\""
../configure ${CONFIG_PARA} --extra-cflags="${CFLAGS}"

echo "make install"
make ${RABBITIM_MAKE_JOB_PARA} && make install

cd $CUR_DIR
