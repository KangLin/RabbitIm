#!/bin/bash
# Author: Kang Lin <kl222@126.com>

#See: https://blog.csdn.net/alwaysbefine/article/details/114187380
set -x
set -e
#set -v

if [ -z "$BUILD_VERBOSE" ]; then
    BUILD_VERBOSE=OFF
fi

source $(dirname $(readlink -f $0))/common.sh

# modify follow value
Qt6_DIR=/opt/Qt/6.10.3/android_x86_64
DEPENDENT_INSTALL_DIR=/data/tigervnc/build_android/install
BUILD_TYPE=Debug
VCPKG_TARGET_TRIPLET=x64-android
VCPKG_ROOT=/data/vcpkg

OLD_DIR=`pwd`
mkdir -p build_android
cd build_android
${Qt6_DIR}/bin/qt-cmake .. \
    -DCMARK_SHARED=OFF \
    -DCMARK_TESTS=OFF \
    -DCMARK_STATIC=ON \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DCMAKE_AUTOGEN_VERBOSE=ON \
    -DCMAKE_MAKE_PROGRAM=ninja \
    -DQT_CHAINLOAD_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
    -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake \
    -DVCPKG_TARGET_TRIPLET=${VCPKG_TARGET_TRIPLET} \
    -DX_VCPKG_APPLOCAL_DEPS_INSTALL=ON \
    -DVCPKG_VERBOSE=ON \
    -DRABBIT_ENABLE_INSTALL_DEPENDENT=ON \
    -DRABBIT_ENABLE_INSTALL_QT=ON \
    -DRABBIT_ENABLE_INSTALL_TO_BUILD_PATH=ON \
    -DQT_HOST_PATH=${Qt6_DIR}/../gcc_64 \
    -DQT_ANDROID_SIGN_APK=ON \
    -DQT_ENABLE_VERBOSE_DEPLOYMENT=ON \
    -DVCPKG_TRACE_FIND_PACKAGE=ON \
    -DQt6LinguistTools_DIR=${Qt6_DIR}/../gcc_64/lib/cmake/Qt6LinguistTools \
    -DCMAKE_INSTALL_PREFIX=`pwd`/install \
    -DQXmpp_DIR=${DEPENDENT_INSTALL_DIR}/lib/cmake/qxmpp
cmake --build . --config ${BUILD_TYPE} --parallel $(nproc)
cmake --install . --config ${BUILD_TYPE}
cd $OLD_DIR
