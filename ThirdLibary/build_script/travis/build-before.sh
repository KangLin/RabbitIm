#!/bin/sh
#下载编译好的第三方依赖库

set -ev

function function_common()
{
    #下载最新cmake程序
    if [ -n "$CMAKE" ]; then
        wget http://www.cmake.org/files/v3.1/cmake-3.1.0-Linux-x86_64.tar.gz
        tar xf cmake-3.1.0-Linux-x86_64.tar.gz
        mv cmake-3.1.0-Linux-x86_64 cmake
    fi
}

function function_android()
{
    function_common
    
    #下载android ndk
    wget http://dl.google.com/android/ndk/android-ndk-r9c-linux-x86_64.tar.bz2
    tar xf android-ndk-r9c-linux-x86_64.tar.bz2
    mv android-ndk-r9c android-ndk

    #wget https://dl.google.com/android/ndk/android-ndk-r10d-linux-x86_64.bin
    #chmod a+x android-ndk-r10d-linux-x86_64.bin
    #./android-ndk-r10d-linux-x86_64.bin > /dev/null
    #mv android-ndk-r10d android-ndk
    #export ANDROID_NDK_ROOT=`pwd`/android-ndk

    #下载 android sdk
    wget http://182.254.185.29/download/travis/android-sdk.tar.gz 
    tar xzf android-sdk.tar.gz 
    mv sdk android-sdk
    
    #下载qt for android
    wget http://182.254.185.29/download/travis/qt_android.tar.gz
    tar xzf qt_android.tar.gz 
    mv android_armv7 qt
    
    #下载第三方依赖库
    wget http://182.254.185.29/download/travis/android.tar.gz
    tar xzf android.tar.gz 
}

function function_unix()
{
    function_common
     #下载qt for linux
    wget http://182.254.185.29/download/travis/qt_linux.tar.gz
    tar xzf qt_linux.tar.gz 
    mv gcc_64 qt
    
    #下载第三方依赖库
    wget http://182.254.185.29/download/travis/unix.tar.gz
    tar xzf unix.tar.gz
}

function function_mingw()
{
    function_common
    cd ${SOURCE_DIR}/ThirdLibary/build_script
    echo "pwd:`pwd`"
    echo "Source_dir:${SOURCE_DIR}"
    source build.sh ${BUILD_TARGERT}
    cd ${SOURCE_DIR}/ThirdLibary
}

SOURCE_DIR="`pwd`"
echo $SOURCE_DIR
cd ThirdLibary

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

cd ${SOURCE_DIR}
