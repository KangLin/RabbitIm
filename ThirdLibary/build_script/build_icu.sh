#!/bin/sh

#作者：康林
#参数:
#    $1:编译目标(android、windows_msvc、windows_mingw、unix)
#    $2:源码的位置 

#运行本脚本前,先运行 build_$1_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix）
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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/icu
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    #echo "git clone https://github.com/svn2github/libicu.git ${RABBITIM_BUILD_SOURCE_CODE}/source"
    #git clone https://github.com/svn2github/libicu.git ${RABBITIM_BUILD_SOURCE_CODE}
    echo "svn co http://source.icu-project.org/repos/icu/icu/trunk/ ${RABBITIM_BUILD_SOURCE_CODE}"
    svn co http://source.icu-project.org/repos/icu/icu/trunk/ ${RABBITIM_BUILD_SOURCE_CODE}
fi

SOURCE_DIR=${RABBITIM_BUILD_SOURCE_CODE}/source     #源代码目录
CONFIG_DIR=${RABBITIM_BUILD_SOURCE_CODE}/temp_${RABBITIM_BUILD_TARGERT}_Config #配置目录
BUILD_DIR=${RABBITIM_BUILD_SOURCE_CODE}/temp_${RABBITIM_BUILD_TARGERT}_Build   #编译目录
rm -fr ${CONFIG_DIR} ${BUILD_DIR}
mkdir -p ${BUILD_DIR} ${CONFIG_DIR}

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
    ;;
    unix)
        cd ${CONFIG_DIR}
        ${SOURCE_DIR}/runConfigureICU Linux/gcc --prefix=${RABBITIM_BUILD_PREFIX}
        make ${RABBITIM_MAKE_JOB_PARA} && make install
        ;;
    windows_msvc)
        cd ${CONFIG_DIR}
        ${SOURCE_DIR}/runConfigureICU MSYS/MSVC --prefix=${RABBITIM_BUILD_PREFIX}
        make ${RABBITIM_MAKE_JOB_PARA} \
            && make install \
            && cp -lf ${RABBITIM_BUILD_PREFIX}/lib/icu*.dll ${RABBITIM_BUILD_PREFIX}/bin/.
        ;;
    windows_mingw)
        case `uname -s` in
            Linux*|Unix*)
                cd ${CONFIG_DIR}
                ${SOURCE_DIR}/runConfigureICU MinGW
                make ${RABBITIM_MAKE_JOB_PARA}
                cd ${BUILD_DIR}
                ${SOURCE_DIR}/configure --host=${RABBITIM_BUILD_CROSS_HOST} --with-cross_build=${CONFIG_DIR} --prefix=${RABBITIM_BUILD_PREFIX}
                make ${RABBITIM_MAKE_JOB_PARA} \
                    && make install \
                    && cp -lf ${RABBITIM_BUILD_PREFIX}/lib/icu*.dll ${RABBITIM_BUILD_PREFIX}/bin/.
                ;;
            MINGW*|MSYS*)
                cd ${CONFIG_DIR}
                ${SOURCE_DIR}/runConfigureICU MinGW --prefix=${RABBITIM_BUILD_PREFIX}
                make ${RABBITIM_MAKE_JOB_PARA} \
                    && make install \
                    && cp -lf ${RABBITIM_BUILD_PREFIX}/lib/icu*.dll ${RABBITIM_BUILD_PREFIX}/bin/.
                ;;
        esac
        ;;
    *)
        echo "${HELP_STRING}"
        exit 2
        ;;
esac

cd $CUR_DIR
