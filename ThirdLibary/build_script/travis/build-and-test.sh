#!/bin/sh
set -e

function function_common()
{
    source build_rabbitim.sh $BUILD_TARGERT `pwd`/../.. $QMAKE
}

function function_android()
{
    export ANDROID_NDK_ROOT=`pwd`/ThirdLibary/android-ndk
    export ANDROID_NDK=$ANDROID_NDK_ROOT
    export ANDROID_SDK_ROOT=`pwd`/ThirdLibary/android-sdk
    export ANDROID_SDK=$ANDROID_SDK_ROOT
    #export CC=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc
    #export CXX=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-g++
    
    function_common    
}

function function_unix()
{
    function_common
}

function function_mingw()
{
    function_common
}

echo "pwd:`pwd`"
export PATH=`pwd`/ThirdLibary/cmake/bin:$PATH
cd `pwd`/ThirdLibary/build_script
    
case ${BUILD_TARGERT} in
    android)
        function_android
        ;;
    unix)
        function_unix
        ;;
    unix_mingw)
        function_mingw
        ;;
    *)
        ;;
esac


