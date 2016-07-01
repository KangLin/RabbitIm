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
HELP_STRING="Usage $0 PLATFORM(android|windows_msvc|windows_mingw|unix) [SOURCE_CODE_ROOT_DIRECTORY]"

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
	CURL_FILE=curl-7_49_1
    if [ "TRUE" = "${RABBITIM_USE_REPOSITORIES}" ]; then
        echo "git clone -q  git://github.com/bagder/curl.git ${RABBITIM_BUILD_SOURCE_CODE}"
        #git clone -q  --branch=$CURL_FILE git://github.com/bagder/curl.git ${RABBITIM_BUILD_SOURCE_CODE}
        git clone  -q --branch=$CURL_FILE git://github.com/bagder/curl.git ${RABBITIM_BUILD_SOURCE_CODE}
    else
        echo "wget  -q https://github.com/bagder/curl/archive/${CURL_FILE}.zip"
        mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        wget -c -q https://github.com/bagder/curl/archive/${CURL_FILE}.zip
        unzip -q  ${CURL_FILE}.zip
        mv curl-${CURL_FILE} ..
        rm -fr *
        cd ..
        rm -fr ${RABBITIM_BUILD_SOURCE_CODE}
        mv -f curl-${CURL_FILE} ${RABBITIM_BUILD_SOURCE_CODE}
    fi
fi

cd ${RABBITIM_BUILD_SOURCE_CODE}

if [ "$RABBITIM_CLEAN" ]; then
    if [ -d ".git" ]; then
        echo "git clean -xdf"
        git clean -xdf
    fi
fi

if [ ! -f configure ]; then
    echo "sh buildconf"
    if [ "${RABBITIM_BUILD_TARGERT}" != "windows_msvc" ]; then
        ./buildconf
    fi
fi

if [ "${RABBITIM_BUILD_TARGERT}" = "windows_msvc" ]; then
    if [ -n "$RABBITIM_CLEAN" ]; then
        rm -fr builds
    fi
#else
#    mkdir -p build_${RABBITIM_BUILD_TARGERT}
#    cd build_${RABBITIM_BUILD_TARGERT}
#    if [ -n "$RABBITIM_CLEAN" ]; then
#        rm -fr *
#    fi
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
if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
    CONFIG_PARA="--enable-static --disable-shared"
    LDFLAGS="-static"
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
        CONFIG_PARA="$CONFIG_PARA --host=${RABBITIM_BUILD_CROSS_HOST} "
        CONFIG_PARA="${CONFIG_PARA} --with-sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        CFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        CPPFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        LDFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        ;;
    unix)
        CONFIG_PARA="${CONFIG_PARA} --with-gnu-ld --enable-sse"
        ;;
    windows_msvc)
        #cmake .. -G"Visual Studio 12 2013" \
        #    -DCMAKE_INSTALL_PREFIX="$RABBITIM_BUILD_PREFIX" \
        #    -DCMAKE_BUILD_TYPE="Release" \
        #    -DBUILD_CURL_TESTS=OFF \
        #    -DCURL_STATICLIB=OFF
        #;;
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        ./buildconf.bat
        cd winbuild
        if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
            MODE=static
        else
            MODE=dll
        fi
        nmake -f Makefile.vc mode=$MODE VC=${VC_TOOLCHAIN} WITH_DEVEL=$RABBITIM_BUILD_PREFIX
        cp -fr ${RABBITIM_BUILD_SOURCE_CODE}/builds/libcurl-vc${VC_TOOLCHAIN}-x86-release-${MODE}-ipv6-sspi-winssl/* ${RABBITIM_BUILD_PREFIX}
        cd $CUR_DIR
        exit 0
        ;;
    windows_mingw)
        CONFIG_PARA="${CONFIG_PARA} --enable-sse "
        case `uname -s` in
            Linux*|Unix*|CYGWIN*)
                CONFIG_PARA="${CONFIG_PARA} CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc --host=${RABBITIM_BUILD_CROSS_HOST}"
                ;;
            MINGW* | MSYS*)
                CONFIG_PARA="${CONFIG_PARA} --host=$RABBITIM_BUILD_CROSS_HOST"
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
CONFIG_PARA="${CONFIG_PARA} --prefix=${RABBITIM_BUILD_PREFIX} --disable-manual --enable-verbose"
#CONFIG_PARA="${CONFIG_PARA} --enable-libgcc  "
CONFIG_PARA="${CONFIG_PARA} --with-ssl=${RABBITIM_BUILD_PREFIX}"  #--with-sysroot=${RABBITIM_BUILD_PREFIX}"
if [ "${RABBITIM_BUILD_TARGERT}" = android ]; then
    echo "./configure ${CONFIG_PARA} CFLAGS=\"${CFLAGS=}\" CPPFLAGS=\"${CPPFLAGS}\" LDFLAGS=\"${LDFLAGS}\""
    ./configure ${CONFIG_PARA} CFLAGS="${CFLAGS}" CPPFLAGS="${CPPFLAGS}" LDFLAGS="${LDFLAGS}"
else
    echo "./configure ${CONFIG_PARA} LDFLAGS=\"${LDFLAGS}\""
    ./configure ${CONFIG_PARA} LDFLAGS="${LDFLAGS}"
fi

${MAKE} V=1 && ${MAKE} install

cd $CUR_DIR
