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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/osg
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    OSG_VERSION=OpenSceneGraph-3.5.3
    if [ "TRUE" = "${RABBITIM_USE_REPOSITORIES}" ]; then
        echo "git clone -q --branch=${OSG_VERSION} https://github.com/openscenegraph/OpenSceneGraph.git ${RABBITIM_BUILD_SOURCE_CODE}"
        git clone -q --branch=$OSG_VERSION https://github.com/openscenegraph/OpenSceneGraph.git ${RABBITIM_BUILD_SOURCE_CODE}
    else
        echo "wget -q https://github.com/openscenegraph/OpenSceneGraph/archive/${OSG_VERSION}.zip"
        mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        wget -q https://github.com/openscenegraph/OpenSceneGraph/archive/${OSG_VERSION}.zip
        unzip -q ${OSG_VERSION}.zip
        mv OpenSceneGraph-${OSG_VERSION} ..
        rm -fr *
        cd ..
        rm -fr ${RABBITIM_BUILD_SOURCE_CODE}
        mv -f OpenSceneGraph-${OSG_VERSION} ${RABBITIM_BUILD_SOURCE_CODE}
    fi
fi

RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_SOURCE_CODE}
cd ${RABBITIM_BUILD_SOURCE_CODE}

mkdir -p build_${RABBITIM_BUILD_TARGERT}
cd build_${RABBITIM_BUILD_TARGERT}
if [ -n "$RABBITIM_CLEAN" ]; then
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

#需要设置 CMAKE_MAKE_PROGRAM 为 make 程序路径。

if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
    CMAKE_PARA="-DDYNAMIC_OPENSCENEGRAPH=OFF" 
else
    CMAKE_PARA="-DDYNAMIC_OPENSCENEGRAPH=ON"
fi
MAKE_PARA="-- ${RABBITIM_MAKE_JOB_PARA} VERBOSE=1"
case ${RABBITIM_BUILD_TARGERT} in
    android)
        export ANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-${RABBITIM_BUILD_TOOLCHAIN_VERSION}
        export ANDROID_NATIVE_API_LEVEL=android-${RABBITIM_BUILD_PLATFORMS_VERSION}
        CMAKE_PARA="-DBUILD_SHARED_LIBS=OFF -DDYNAMIC_OPENTHREADS=OFF"
        CMAKE_PARA="${CMAKE_PARA} -DDYNAMIC_OPENSCENEGRAPH=OFF -DCMAKE_TOOLCHAIN_FILE=${RABBITIM_BUILD_SOURCE_CODE}/PlatformSpecifics/Android/android.toolchain.cmake"
        CMAKE_PARA="${CMAKE_PARA} -DBUILD_OSG_APPLICATIONS=OFF -DOSG_BUILD_PLATFORM_ANDROID=ON"
        CMAKE_PARA="${CMAKE_PARA} -DANDROID_NATIVE_API_LEVEL=android-${RABBITIM_BUILD_PLATFORMS_VERSION}"
        CMAKE_PARA="${CMAKE_PARA} -DANDROID_TOOLCHAIN_NAME=${RABBITIM_BUILD_CROSS_HOST}-${RABBITIM_BUILD_TOOLCHAIN_VERSION}"
        CMAKE_PARA="${CMAKE_PARA} -DANDROID_NDK_ABI_NAME=${ANDROID_NDK_ABI_NAME}"
        CMAKE_PARA="${CMAKE_PARA} -DOSG_GL1_AVAILABLE=OFF -DOSG_GL2_AVAILABLE=OFF -DOSG_GL3_AVAILABLE=OFF -DOSG_GLES1_AVAILABLE=OFF"
        CMAKE_PARA="${CMAKE_PARA} -DOSG_GLES2_AVAILABLE=ON -DOSG_GL_LIBRARY_STATIC=OFF -DOSG_GL_DISPLAYLISTS_AVAILABLE=OFF"
        CMAKE_PARA="${CMAKE_PARA} -DOSG_GL_MATRICES_AVAILABLE=OFF -DOSG_GL_VERTEX_FUNCS_AVAILABLE=OFF -DOSG_GL_VERTEX_ARRAY_FUNCS_AVAILABLE=OFF -DOSG_GL_FIXED_FUNCTION_AVAILABLE=OFF"
        CMAKE_PARA="${CMAKE_PARA} -DBUILD_OSG_APPLICATIONS=OFF -DBUILD_OSG_PACKAGES=OFF"
        if [ -n "$RABBITIM_CMAKE_MAKE_PROGRAM" ]; then
            CMAKE_PARA="${CMAKE_PARA} -DCMAKE_MAKE_PROGRAM=$RABBITIM_CMAKE_MAKE_PROGRAM" 
        fi
        ;;
    unix)
        ;;
    windows_msvc)
        #GENERATORS="Visual Studio 12 2013"
        export OSG_3RDPARTY_DIR=$RABBITIM_BUILD_PREFIX
        ;;
    windows_mingw)
        case `uname -s` in
            Linux*|Unix*|CYGWIN*)
                CMAKE_PARA="${CMAKE_PARA} -DCMAKE_TOOLCHAIN_FILE=$RABBITIM_BUILD_PREFIX/../../cmake/platforms/toolchain-mingw.cmake"
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

CMAKE_PARA="${CMAKE_PARA} -DBUILD_DOCUMENTATION=OFF -DBUILD_OSG_EXAMPLES=OFF" # -DBUILD_OSG_APPLICATIONS=OFF"
CMAKE_PARA="${CMAKE_PARA} -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5"

echo "cmake .. -DCMAKE_INSTALL_PREFIX=$RABBITIM_BUILD_PREFIX -DCMAKE_BUILD_TYPE=Release -G\"${GENERATORS}\" ${CMAKE_PARA}"
cmake .. \
    -DCMAKE_INSTALL_PREFIX="$RABBITIM_BUILD_PREFIX" \
    -DCMAKE_BUILD_TYPE="Release" \
    -G"${GENERATORS}" ${CMAKE_PARA}

cmake --build . --target install --config Release #${MAKE_PARA}

cd $CUR_DIR
