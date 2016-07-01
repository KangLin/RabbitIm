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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/openssl
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    OPENSLL_BRANCH=OpenSSL_1_0_2h
    if [ "TRUE" = "${RABBITIM_USE_REPOSITORIES}" ]; then
        echo "git clone -q --branch=${OPENSLL_BRANCH} https://github.com/openssl/openssl  ${RABBITIM_BUILD_SOURCE_CODE}"
        git clone -q --branch=${OPENSLL_BRANCH} https://github.com/openssl/openssl ${RABBITIM_BUILD_SOURCE_CODE}
        #git clone -q https://github.com/openssl/openssl ${RABBITIM_BUILD_SOURCE_CODE}
    else
        mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        echo "wget -q https://github.com/openssl/openssl/archive/${OPENSLL_BRANCH}.zip"
        wget -q -c https://github.com/openssl/openssl/archive/${OPENSLL_BRANCH}.zip
        unzip -q ${OPENSLL_BRANCH}.zip
        mv openssl-${OPENSLL_BRANCH} ..
        rm -fr *
        cd ..
        rm -fr ${RABBITIM_BUILD_SOURCE_CODE}
        mv -f openssl-${OPENSLL_BRANCH} ${RABBITIM_BUILD_SOURCE_CODE}
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
echo "RABBITIM_BUILD_STATIC:$RABBITIM_BUILD_STATIC"
echo "PATH:$PATH"
echo ""

if [ -n "$RABBITIM_CLEAN" ]; then
    if [ -d ".git" ]; then
        git clean -xdf
    else
        if [ -f Makefile ]; then
            ${MAKE} distclean
        fi
    fi
fi

if [ "$RABBITIM_BUILD_STATIC" != "static" ]; then
    MODE=shared
else
    MODE="no-shared no-pic"
fi

echo "configure ..."
case ${RABBITIM_BUILD_TARGERT} in
    android)
        #export ANDROID_DEV="${RABBITIM_BUILD_CROSS_SYSROOT}/usr"
        export LDFLAGS="--sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        CFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        perl Configure --cross-compile-prefix=${RABBITIM_BUILD_CROSS_PREFIX} \
                --prefix=${RABBITIM_BUILD_PREFIX} \
                --openssldir=${RABBITIM_BUILD_PREFIX} \
                $MODE \
                android-armv7  --sysroot="${RABBITIM_BUILD_CROSS_SYSROOT}" -march=armv7-a -mfpu=neon
        ;;
    unix)
        ./config --prefix=${RABBITIM_BUILD_PREFIX} --openssldir=${RABBITIM_BUILD_PREFIX} $MODE
        ;;
    windows_msvc)
        perl Configure \
            --prefix=${RABBITIM_BUILD_PREFIX} \
            --openssldir=${RABBITIM_BUILD_PREFIX} \
            VC-WIN32
        ms/do_nasm.bat
        echo "make install"
        if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
            #静态库  
            nmake -f ms/nt.mak install
        else
            #动态库  
            nmake -f ms/ntdll.mak install
        fi
        cd $CUR_DIR
        exit 0
        ;;
    windows_mingw)
        case `uname -s` in
            MINGW*|MSYS*)
                perl Configure --prefix=${RABBITIM_BUILD_PREFIX} \
                    --openssldir=${RABBITIM_BUILD_PREFIX} \
                    $MODE mingw
                ;;
            Linux*|Unix*|CYGWIN*|*)
                perl Configure --prefix=${RABBITIM_BUILD_PREFIX} \
                    --openssldir=${RABBITIM_BUILD_PREFIX} \
                    --cross-compile-prefix=${RABBITIM_BUILD_CROSS_PREFIX} \
                    $MODE mingw
                ;;
        esac
        ;;
    *)
        echo "${HELP_STRING}"
        cd $CUR_DIR
        exit 3
        ;;
esac

echo "${MAKE} depend"
${MAKE} depend
echo "make install"
${MAKE} 
${MAKE} install

cd $CUR_DIR
