#!/bin/sh

#作者：康林
#参数:
#    $1:编译目标(android、windows_msvc、windows_mingw、unix)
#    $2:源码的位置 

#运行本脚本前,先运行 build_$1_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix)
#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_SOURCE_CODE    #源码目录
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/libcurl
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    if [ -n "$RABBITIM_USE_REPOSITORIES" ]; then
        echo "git clone git://github.com/bagder/curl.git ${RABBITIM_BUILD_SOURCE_CODE}"
        #git clone --branch=curl-7_41_0 git://github.com/bagder/curl.git ${RABBITIM_BUILD_SOURCE_CODE}
        git clone git://github.com/bagder/curl.git ${RABBITIM_BUILD_SOURCE_CODE}
    else
        CUR_FILE=curl-7.41.0
        echo "wget http://curl.haxx.se/download/${CUR_FILE}.tar.gz"
        mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        wget http://curl.haxx.se/download/${CUR_FILE}.tar.gz
        tar xzf ${CUR_FILE}.tar.gz
        RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_SOURCE_CODE}/${CUR_FILE}
    fi
fi

cd ${RABBITIM_BUILD_SOURCE_CODE}

if [ ! -f configure -a "${RABBITIM_BUILD_TARGERT}" != "windows_msvc" ]; then
    echo "sh buildconf"
    sh buildconf
fi

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

CONFIG_PARA="--disable-static --enable-shared"
case ${RABBITIM_BUILD_TARGERT} in
    android)
        CONFIG_PARA="CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc --disable-shared -enable-static --host=${RABBITIM_BUILD_CROSS_HOST}"
        CFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        CPPFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
		;;
    unix)
        CONFIG_PARA="${CONFIG_PARA} --with-gnu-ld --enable-sse "
		;;
    windows_msvc)
        cmake .. -G"Visual Studio 12 2013" \
            -DCMAKE_INSTALL_PREFIX="$RABBITIM_BUILD_PREFIX" \
            -DCMAKE_BUILD_TYPE="Release" \
            -DBUILD_CURL_TESTS=OFF \
            -DCURL_STATICLIB=OFF
        ;;
    windows_mingw)
		CONFIG_PARA="${CONFIG_PARA}  CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc --host=${RABBITIM_BUILD_CROSS_HOST} --enable-sse "
		;;
    *)
		echo "${HELP_STRING}"
        cd $CUR_DIR
		exit 3
		;;
esac

echo "make install"
if [ "${RABBITIM_BUILD_TARGERT}" = "windows_msvc" ]; then
    cmake --build . --target install --config Release
else
    CONFIG_PARA="${CONFIG_PARA} --prefix=$RABBITIM_BUILD_PREFIX --disable-debug --disable-curldebug --disable-manual"
    CONFIG_PARA="${CONFIG_PARA} --with-ssl=$RABBITIM_BUILD_PREFIX" 
    echo "../configure ${CONFIG_PARA} CFLAGS=\"${CFLAGS=}\" CPPFLAGS=\"${CPPFLAGS}\""
    ../configure ${CONFIG_PARA} CFLAGS="${CFLAGS}" CPPFLAGS="${CPPFLAGS}"
    
    make ${RABBITIM_MAKE_JOB_PARA} && make install
fi

cd $CUR_DIR
