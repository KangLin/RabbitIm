#!/bin/sh
set -ev

if [ -n "$ANDROID_TARGET" ]; then #编译 android
    export ANDROID_NDK_ROOT=`pwd`/ThirdLibary/android-ndk-r9c
    export CC=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc
    export CXX=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-g++
fi

QMAKE_ARGS="QXMPP_USE_VPX=1 RABBITIM_USER_FFMPEG=1 RABBITIM_USER_LIBCURL=1 RABBITIM_USER_OPENSSL=1"

if [ -n "$CC" ]; then
    QMAKE_ARGS="$QMAKE_ARGS QMAKE_CC=$CC"
fi

if [ -n "$CXX" ]; then
    QMAKE_ARGS="$QMAKE_ARGS QMAKE_CXX=$CXX"
fi

# compile
if [ -n "$QMAKE" ]; then
    echo "$QMAKE $QMAKE_ARGS"
    $QMAKE $QMAKE_ARGS
    make VERBOSE=1
fi

#用cmake编译
if [ -n "$CMAKE" ]; then
    $CMAKE . -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DQt5_DIR=${Qt5_DIR} 
    $CMAKE  --build . --config Release
fi
