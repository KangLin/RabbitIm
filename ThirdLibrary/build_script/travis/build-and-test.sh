#!/bin/bash
set -e

function function_common()
{
    #echo "PATH:$PATH"
    cd ${SOURCE_DIR}/ThirdLibrary/build_script
    ./build_rabbitim.sh $BUILD_TARGERT ${SOURCE_DIR} $QMAKE
}

function function_android()
{
    export ANDROID_NDK_ROOT=${SOURCE_DIR}/ThirdLibrary/android-ndk
    export ANDROID_NDK=$ANDROID_NDK_ROOT
    export ANDROID_SDK_ROOT=${SOURCE_DIR}/ThirdLibrary/android-sdk
    export ANDROID_SDK=$ANDROID_SDK_ROOT
    #export CC=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc
    #export CXX=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-g++

    function_common
}

function function_unix()
{
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SOURCE_DIR}/ThirdLibrary/unix/qt/lib
    function_common
}

function function_mingw()
{
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SOURCE_DIR}/ThirdLibrary/windows_mingw/qt/lib
    function_common
}

SOURCE_DIR=`pwd`
echo $SOURCE_DIR

if [ "$QMAKE" = "cmake" ]; then
    export PATH=`pwd`/ThirdLibrary/cmake/bin:$PATH
fi

case ${BUILD_TARGERT} in
    android)
        function_android
        ;;
    unix)
        function_unix
        ;;
    windows_mingw)
        function_mingw
        ;;
    *)
        ;;
esac
