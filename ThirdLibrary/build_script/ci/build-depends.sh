#!/bin/bash
#下载编译好的第三方依赖库

set -e

function function_install_yasm()
{
	#安装 yasm
	mkdir -p ${SOURCE_DIR}/ThirdLibrary/src
    cd ${SOURCE_DIR}/ThirdLibrary/src
	wget http://www.tortall.net/projects/yasm/releases/yasm-1.3.0.tar.gz 
	tar xzf yasm-1.3.0.tar.gz 
	cd yasm-1.3.0/
	./configure && sudo make install -j2
	cd ${SOURCE_DIR}
}

function function_common()
{
    #下载最新cmake程序
    if [ "cmake" = "${QMAKE}" ]; then
        cd ${SOURCE_DIR}/ThirdLibrary
        wget --no-check-certificate http://www.cmake.org/files/v3.1/cmake-3.1.0-Linux-x86_64.tar.gz
        tar xf cmake-3.1.0-Linux-x86_64.tar.gz
        mv cmake-3.1.0-Linux-x86_64 cmake
        cd ${SOURCE_DIR}
    fi

    if [ "true" = "${RABBITIM_BUILD_THIRDLIBRARY}" ]; then
		#编译第三方库
		cd ${SOURCE_DIR}/ThirdLibrary/build_script
		./build.sh ${BUILD_TARGERT} ${SOURCE_DIR}/ThirdLibrary/src > /dev/null
	fi
}

function function_android()
{
    cd ${SOURCE_DIR}/ThirdLibrary

    #下载android ndk
    wget http://dl.google.com/android/ndk/android-ndk-r10e-linux-x86_64.bin
    sudo chmod u+x android-ndk-r10e-linux-x86_64.bin
    ./android-ndk-r10e-linux-x86_64.bin > /dev/null
    mv android-ndk-r10e android-ndk
    export ANDROID_NDK_ROOT=${SOURCE_DIR}/ThirdLibrary/android-ndk
    export ANDROID_NDK=$ANDROID_NDK_ROOT
 
    #下载 android sdk
    #wget -q http://182.254.185.29/download/travis/android-sdk.tar.gz 
    #tar xzf android-sdk.tar.gz 
    #mv sdk android-sdk
 
    export ANDROID_SDK_ROOT=${SOURCE_DIR}/ThirdLibrary/android-sdk
    export ANDROID_SDK=$ANDROID_SDK_ROOT

	function_common
	cd ${SOURCE_DIR}/ThirdLibrary
}

function function_unix()
{
    #汇编工具yasm
    function_install_yasm

    function_common

    cd ${SOURCE_DIR}/ThirdLibrary
}

function function_mingw()
{
    #汇编工具yasm
    function_install_yasm

    cd ${SOURCE_DIR}/ThirdLibrary
    if [ "true" == "$RABBITIM_BUILD_THIRDLIBRARY" ]; then
        export RABBITIM_BUILD_CROSS_HOST=i586-mingw32msvc
    fi

    function_common
    cd ${SOURCE_DIR}/ThirdLibrary
}

SOURCE_DIR="`pwd`"
echo $SOURCE_DIR

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
    echo "There aren't ${BUILD_TARGERT}"
        ;;
esac

cd ${SOURCE_DIR}
