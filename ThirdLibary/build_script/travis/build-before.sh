#!/bin/bash
#下载编译好的第三方依赖库

set -e

function function_install_yasm()
{
	#安装 yasm
	mkdir -p ${SOURCE_DIR}/ThirdLibary/src
    cd ${SOURCE_DIR}/ThirdLibary/src
	wget http://www.tortall.net/projects/yasm/releases/yasm-1.3.0.tar.gz 
	tar xzvf yasm-1.3.0.tar.gz 
	cd yasm-1.3.0/
	./configure && sudo make install -j2
	cd ${SOURCE_DIR}
}

function function_common()
{
    #下载最新cmake程序
    if [ "cmake" = "${QMAKE}" ]; then
        cd ${SOURCE_DIR}/ThirdLibary
        wget http://www.cmake.org/files/v3.1/cmake-3.1.0-Linux-x86_64.tar.gz
        tar xf cmake-3.1.0-Linux-x86_64.tar.gz
        mv cmake-3.1.0-Linux-x86_64 cmake
        cd ${SOURCE_DIR}
    fi
    
    if [ -n "${BUILD_THIRDLIBARY}" ]; then
		#编译第三方库
		cd ${SOURCE_DIR}/ThirdLibary/build_script
		sh build.sh ${BUILD_TARGERT} ${SOURCE_DIR}/../src
	fi
}

function function_android()
{
    cd ${SOURCE_DIR}/ThirdLibary
    
    #下载android ndk
    wget http://dl.google.com/android/ndk/android-ndk-r9c-linux-x86_64.tar.bz2
    tar xf android-ndk-r9c-linux-x86_64.tar.bz2
    mv android-ndk-r9c android-ndk

    #下载 android sdk
    wget http://182.254.185.29/download/travis/android-sdk.tar.gz 
    tar xzf android-sdk.tar.gz 
    mv sdk android-sdk
 
	function_common
	cd ${SOURCE_DIR}/ThirdLibary
	
	if [ -z "$BUILD_THIRDLIBARY" ]; then
		#下载第三方依赖库
		wget http://182.254.185.29/download/travis/android.tar.gz
		tar xzf android.tar.gz 
    fi
    
    cd ${SOURCE_DIR}/ThirdLibary
}

function function_unix()
{
    #汇编工具yasm
    function_install_yasm
	
    function_common
    
    cd ${SOURCE_DIR}/ThirdLibary
    
    if [ -z "$BUILD_THIRDLIBARY" ]; then
        #下载第三方依赖库
        wget http://182.254.185.29/download/travis/unix.tar.gz
        tar xzf unix.tar.gz
    fi
    cd ${SOURCE_DIR}/ThirdLibary
  
}

function function_mingw()
{
    #汇编工具yasm
    function_install_yasm

    cd ${SOURCE_DIR}/ThirdLibary
    if [ -z "$BUILD_THIRDLIBARY" ]; then
        echo "Download third library"
        #下载第三方依赖库
        wget http://182.254.185.29/download/travis/windows_mingw.tar.gz
        tar xzf windows_mingw.tar.gz
    else
        export RABBITIM_BUILD_CROSS_HOST=i586-mingw32msvc
    fi
    
    function_common
    cd ${SOURCE_DIR}/ThirdLibary
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
        ;;
esac

cd ${SOURCE_DIR}
