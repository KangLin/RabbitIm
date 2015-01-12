#!/bin/sh
set -ev

if [ -n "$ANDROID_TARGET" ]; then #编译 android
    export ANDROID_NDK_ROOT=`pwd`/ThirdLibary/android-ndk
    export ANDROID_NDK=$ANDROID_NDK_ROOT
    export ANDROID_SDK_ROOT=`pwd`/ThirdLibary/android-sdk
    export ANDROID_SDK=$ANDROID_SDK_ROOT
    export CC=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc
    export CXX=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-g++
fi

# compile
if [ -n "$QMAKE" ]; then
    QMAKE_ARGS="QXMPP_USE_VPX=1 RABBITIM_USER_FFMPEG=1 RABBITIM_USER_LIBCURL=1 RABBITIM_USER_OPENSSL=1"

    if [ -n "$CC" ]; then
        QMAKE_ARGS="$QMAKE_ARGS QMAKE_CC=$CC"
    fi

    if [ -n "$CXX" ]; then
       QMAKE_ARGS="$QMAKE_ARGS QMAKE_CXX=$CXX"
    fi

    echo "$QMAKE $QMAKE_ARGS"
    $QMAKE $QMAKE_ARGS
    make VERBOSE=1
fi

#用cmake编译
if [ -n "$CMAKE" ]; then
    if [ -n "$ANDROID_TARGET" ]; then #android需要指定cmake_toolchain_file
        CMAKE_ARGS="-DCMAKE_TOOLCHAIN_FILE=`pwd`/platforms/android/android.toolchain.cmake -DANDROID_ABI=armeabi-v7a -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.8"
    fi

    $CMAKE . -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DQt5_DIR=${Qt5_DIR} ${CMAKE_ARGS}
    $CMAKE  --build . 
fi
