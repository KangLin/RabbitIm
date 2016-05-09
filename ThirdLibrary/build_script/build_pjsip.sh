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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/pjsip
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    PJSIP_VERSION=2.4
    if [ "TRUE" = "${RABBITIM_USE_REPOSITORIES}" ]; then
        echo "svn co http://svn.pjsip.org/repos/pjproject/trunk ${RABBITIM_BUILD_SOURCE_CODE}"
        svn co http://svn.pjsip.org/repos/pjproject/trunk ${RABBITIM_BUILD_SOURCE_CODE}
        #echo "git svn clone http://svn.pjsip.org/repos/pjproject/tags/2.4/ ${RABBITIM_BUILD_SOURCE_CODE}"
        #git svn clone http://svn.pjsip.org/repos/pjproject/tags/2.4/ ${RABBITIM_BUILD_SOURCE_CODE}
    else
        echo "wget -q http://www.pjsip.org/release/${PJSIP_VERSION}/pjproject-${PJSIP_VERSION}.tar.bz2"
        mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        wget -q -c  http://www.pjsip.org/release/${PJSIP_VERSION}/pjproject-${PJSIP_VERSION}.tar.bz2
        tar -jxf pjproject-${PJSIP_VERSION}.tar.bz2
        mv pjproject-${PJSIP_VERSION} ..
        rm -fr *
        cd ..
        rm -fr ${RABBITIM_BUILD_SOURCE_CODE}
        mv -f pjproject-${PJSIP_VERSION} ${RABBITIM_BUILD_SOURCE_CODE}
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
echo ""


if [ -n "$RABBITIM_CLEAN" -a -f "build.mak" ]; then
    echo "make clean"
    make dep;make clean
fi

CONFIG_PARA="${CONFIG_PARA} --prefix=$RABBITIM_BUILD_PREFIX"
case ${RABBITIM_BUILD_TARGERT} in
    android)
        echo "#define PJ_CONFIG_ANDROID 1" > ${RABBITIM_BUILD_SOURCE_CODE}/pjlib/include/pj/config_site.h
        echo "#define PJMEDIA_HAS_VIDEO 1" >> ${RABBITIM_BUILD_SOURCE_CODE}/pjlib/include/pj/config_site.h
        echo "#include <pj/config_site_sample.h>" >> ${RABBITIM_BUILD_SOURCE_CODE}/pjlib/include/pj/config_site.h
        export APP_PLATFORM=$ANDROID_API_VERSION
        export ANDROID_NDK_PLATFORM=$ANDROID_API_VERSION
        export TARGET_ABI=armeabi-v7a
        echo "./configure-android --use-ndk-cflags ${CONFIG_PARA}"
        ./configure-android --use-ndk-cflags ${CONFIG_PARA} 
        ;;
    unix)
        ;;
    windows_msvc)
        echo "#define PJMEDIA_HAS_VIDEO 1" > ${RABBITIM_BUILD_SOURCE_CODE}/pjlib/include/pj/config_site.h
        echo "#define PJMEDIA_VIDEO_DEV_HAS_DSHOW 1" >> ${RABBITIM_BUILD_SOURCE_CODE}/pjlib/include/pj/config_site.h
        msbuild.exe pjproject-vs8.sln -m -t:Rebuild -p:Configuration=Release -p:Platform=Win32
        cd $CUR_DIR
        exit 2
        ;;
    windows_mingw)
    case `uname -s` in
        Linux*|Unix*|CYGWIN*)
            export CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc 
            export CXX=${RABBITIM_BUILD_CROSS_PREFIX}g++
            export AR=${RABBITIM_BUILD_CROSS_PREFIX}ar
            export LD=${RABBITIM_BUILD_CROSS_PREFIX}gcc
            export AS=yasm
            export STRIP=${RABBITIM_BUILD_CROSS_PREFIX}strip
            export NM=${RABBITIM_BUILD_CROSS_PREFIX}nm
            ;;
        *)
        ;;
    esac
        echo "#define PJMEDIA_HAS_VIDEO 1" > ${RABBITIM_BUILD_SOURCE_CODE}/pjlib/include/pj/config_site.h
        #echo "#define PJMEDIA_VIDEO_DEV_HAS_DSHOW 1" >> ${RABBITIM_BUILD_SOURCE_CODE}/pjlib/include/pj/config_site.h
        echo "#define PJMEDIA_HAS_FFMPEG 1" >> ${RABBITIM_BUILD_SOURCE_CODE}/pjlib/include/pj/config_site.h
        echo "./configure ${CONFIG_PARA}"
        ./configure ${CONFIG_PARA}
        ;;
    *)
    echo "${HELP_STRING}"
    cd $CUR_DIR
    exit 3
    ;;
esac

echo "make install"
make ${RABBITIM_MAKE_JOB_PARA} VERBOSE=1 && make install

cd $CUR_DIR
