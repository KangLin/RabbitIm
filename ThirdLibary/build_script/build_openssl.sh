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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/openssl
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    OPENSLL_BRANCH=OpenSSL_1_0_2-stable
    if [ -n "$RABBITIM_USE_REPOSITORIES" ]; then
        echo "git clone https://github.com/openssl/openssl  ${RABBITIM_BUILD_SOURCE_CODE}"
        #git clone --branch=${OPENSLL_BRANCH} https://github.com/openssl/openssl ${RABBITIM_BUILD_SOURCE_CODE}
        git clone https://github.com/openssl/openssl ${RABBITIM_BUILD_SOURCE_CODE}
    else
		mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        echo "wget https://github.com/openssl/openssl/archive/${OPENSLL_BRANCH}-stable.zip"
        wget https://github.com/openssl/openssl/archive/${OPENSLL_BRANCH}.zip
        unzip ${OPENSLL_BRANCH}.zip
        RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_SOURCE_CODE}/openssl-${OPENSLL_BRANCH}
    fi
fi

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

if [ -d ".git" ]; then
    git clean -xdf
else
    if [ -f Makefile ]; then
        make distclean
    fi
fi

echo "configure ..."
case ${RABBITIM_BUILD_TARGERT} in
    android)
        export ANDROID_DEV="${RABBITIM_BUILD_CROSS_SYSROOT}/usr"
        perl Configure --cross-compile-prefix=${RABBITIM_BUILD_CROSS_PREFIX} \
                --prefix=${RABBITIM_BUILD_PREFIX} \
                --openssldir=${RABBITIM_BUILD_PREFIX} \
                android-armv7 \
                --sysroot="${RABBITIM_BUILD_CROSS_SYSROOT}"
        ;;
    unix)
        ./config --prefix=${RABBITIM_BUILD_PREFIX} --openssldir=${RABBITIM_BUILD_PREFIX} shared
        ;;
    windows_msvc)
        perl Configure \
            --prefix=${RABBITIM_BUILD_PREFIX} \
            --openssldir=${RABBITIM_BUILD_PREFIX} \
            VC-WIN32
        ms/do_nasm.bat
        echo "make install"
        #静态库
        #nmake -f ms/nt.mak install﻿﻿
        #动态库  
        nmake -f ms/ntdll.mak install
        cd $CUR_DIR
        exit 0
        ;;
    windows_mingw)
        case `uname -s` in
            MINGW*|MSYS*)
                perl Configure  --prefix=${RABBITIM_BUILD_PREFIX} \
                    --openssldir=${RABBITIM_BUILD_PREFIX} \
                    shared mingw
                ;;
            Linux*|Unix*|CYGWIN*|*)
                perl Configure  --prefix=${RABBITIM_BUILD_PREFIX} \
                    --openssldir=${RABBITIM_BUILD_PREFIX} \
                    --cross-compile-prefix=${RABBITIM_BUILD_CROSS_PREFIX} \
                    shared mingw
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
make ${RABBITIM_MAKE_JOB_PARA} && make install

cd $CUR_DIR
