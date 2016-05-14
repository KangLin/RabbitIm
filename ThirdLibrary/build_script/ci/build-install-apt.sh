#!/bin/bash
set -e

function function_common()
{    
    sudo apt-get update -qq

    sudo apt-get install -y build-essential \
         libtool \
         autotools-dev \
         automake \
         autoconf \
         checkinstall \
         check \
         m4 
    sudo apt-get install -y -qq subversion git wget ant
    #文档产生工具
    sudo apt-get install -y -qq doxygen 
    #编译qt webkit 需要
    sudo apt-get install -y -qq bison flex gperf ruby 
    #icu工具
    sudo apt-get install -y -qq icu-devtools libicu-dev 
}

function function_android()
{
    echo "install android ......"
    #sudo apt-get install -y -qq zlib1g:i386 libstdc++6:i386 libc6:i386
}

function function_unix()
{
    sudo apt-get update -qq

    #文档产生工具
    sudo apt-get install -y -qq doxygen 
    
    # 依赖库  
    if [ "$RABBITIM_BUILD_THIRDLIBRARY" = "TRUE" ]; then
        sudo apt-get install -y -qq libvpx-dev \
            libavcodec-dev libavformat-dev libavutil-dev libswscale-dev \
            libqrencode-dev \
            libssl-dev \
            libcurl4-openssl-dev
    fi
    #Libxcb
    sudo apt-get install -y -qq "^libxcb.*" libx11-xcb-dev libxrender-dev libxi-dev
    #QT 需要===============================
    #opengl
    sudo apt-get install -y -qq mesa-common-dev libglu1-mesa-dev
    # libasound2、libgstreamer
    sudo apt-get install -y -qq libasound2-dev
    sudo apt-get install -y -qq libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev
    #QT 需要===============================

    #安装工具
    sudo apt-get install -y -qq dh-make debhelper
    sudo apt-get install -y -qq yasm
}

function function_mingw()
{
	#安装工具
    sudo apt-get install -y -qq nsis
    sudo apt-get install -y -qq mingw-w64
}

function_common

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
