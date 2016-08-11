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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/geos
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    #VERSION=3.5.0
    #if [ "TRUE" = "${RABBITIM_USE_REPOSITORIES}" ]; then
        echo "svn checkout http://svn.osgeo.org/geos/trunk ${RABBITIM_BUILD_SOURCE_CODE}"
        svn checkout http://svn.osgeo.org/geos/trunk ${RABBITIM_BUILD_SOURCE_CODE}
    #else
    #    echo "wget -c -nv http://download.osgeo.org/geos/geos-$VERSION.tar.bz2"
    #    mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
    #    cd ${RABBITIM_BUILD_SOURCE_CODE}
    #    wget -c -nv http://download.osgeo.org/geos/geos-$VERSION.tar.bz2
    #    tar xf geos-$VERSION.tar.bz2
    #    mv geos-$VERSION ..
    #    cd ..
    #    rm -fr ${RABBITIM_BUILD_SOURCE_CODE}
    #    mv geos-$VERSION ${RABBITIM_BUILD_SOURCE_CODE}
    #fi
fi

cd ${RABBITIM_BUILD_SOURCE_CODE}

if [ ! -f configure ]; then
    echo "sh autogen.sh"
    sh autogen.sh
fi

tools/svn_repo_revision.sh

mkdir -p build_${RABBITIM_BUILD_TARGERT}
cd build_${RABBITIM_BUILD_TARGERT}
if [ "$RABBITIM_CLEAN" = "TRUE" ]; then
    rm -fr *
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
MAKE_PARA="-- ${RABBITIM_MAKE_JOB_PARA}"

if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
    CONFIG_PARA="--enable-static --disable-shared"
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
        LIBS="-lstdc++"
        CONFIG_PARA="CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc CXX=${RABBITIM_BUILD_CROSS_PREFIX}g++ LD=${RABBITIM_BUILD_CROSS_PREFIX}ld"
        CONFIG_PARA="${CONFIG_PARA} --disable-shared -enable-static"
        CONFIG_PARA="${CONFIG_PARA} --with-sysroot=${RABBITIM_BUILD_CROSS_SYSROOT} --host=$RABBITIM_BUILD_CROSS_HOST"
        CFLAGS="-march=armv7-a -mfpu=neon --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        CXXFLAGS="-march=armv7-a -mfpu=neon -std=c++0x --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT} ${RABBITIM_BUILD_CROSS_STL_INCLUDE_FLAGS}"
        if [ -n "${RABBITIM_BUILD_CROSS_STL_LIBS}" ]; then
            LDFLAGS="-L${RABBITIM_BUILD_CROSS_STL_LIBS} -lstdc++"
        fi
        CPPFLAGS=${CXXFLAGS}
        ;;
    unix)
        ;;
    windows_msvc)
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        ./autogen.bat
        nmake -f makefile.vc clean
        nmake -f makefile.vc
        cp src/*.dll $RABBITIM_BUILD_PREFIX/bin
        cp src/*.lib $RABBITIM_BUILD_PREFIX/lib
        cp capi/geos_c.h include/geos.h $RABBITIM_BUILD_PREFIX/include
        cp -r include/geos $RABBITIM_BUILD_PREFIX/include/
        cd $CUR_DIR
        exit 0
        ;;
    windows_mingw)
        case `uname -s` in
            Linux*|Unix*|CYGWIN*)
                export CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc 
                export CXX=${RABBITIM_BUILD_CROSS_PREFIX}g++
                export AR=${RABBITIM_BUILD_CROSS_PREFIX}ar
                export LD=${RABBITIM_BUILD_CROSS_PREFIX}ld
                export AS=${RABBITIM_BUILD_CROSS_PREFIX}as
                export STRIP=${RABBITIM_BUILD_CROSS_PREFIX}strip
                export NM=${RABBITIM_BUILD_CROSS_PREFIX}nm
                CONFIG_PARA="${CONFIG_PARA} CC=${RABBITIM_BUILD_CROSS_PREFIX}gcc"
                CONFIG_PARA="${CONFIG_PARA} --host=${RABBITIM_BUILD_CROSS_HOST}"
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

CONFIG_PARA="${CONFIG_PARA} --prefix=$RABBITIM_BUILD_PREFIX "
echo "../configure ${CONFIG_PARA} CFLAGS=\"${CFLAGS=}\" CPPFLAGS=\"${CPPFLAGS}\" LDFLAGS=\"$LDFLAGS\" LIBS=\"$LIBS\""
../configure ${CONFIG_PARA} CFLAGS="${CFLAGS}" \
    CPPFLAGS="${CPPFLAGS}" CXXFLAGS="${CXXFLAGS}" \
    LDFLAGS="$LDFLAGS" LIBS="$LIBS" \
    --disable-python --disable-ruby --disable-php \
    --enable-dependency-tracking

echo "make install"
make ${RABBITIM_MAKE_JOB_PARA} 
make install

cd $CUR_DIR
