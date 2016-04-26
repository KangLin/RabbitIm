#!/bin/bash
set -ev

function function_common()
{
    #sudo add-apt-repository -y ppa:ubuntu-sdk-team/ppa
    #sudo add-apt-repository --yes ppa:kalakris/cmake
    
    sudo apt-get update -qq

    sudo apt-get install  -y -qq autoconf automake make libtool m4 libtool \
        autotools-dev \
        checkinstall \
        check 

    sudo apt-get install -y -qq subversion git wget ant
    #文档产生工具
    sudo apt-get install -y -qq doxygen 
    #编译qt webkit 需要
    sudo apt-get install -y -qq bison flex gperf ruby 

    #icu工具
    #wget http://mirrors.kernel.org/ubuntu/pool/main/i/icu/libicu52_52.1-3_amd64.deb
    #sudo dpkg -i libicu52_52.1-3_amd64.deb #icu库,用于 Unicode and Globalization 支持
    #sudo apt-get install libicu48 -y -qq
    sudo apt-get install -y -qq icu-devtools  libicu-dev 
}

function function_android()
{
    echo "install android ......"
    #sudo apt-get install -y -qq zlib1g:i386 libstdc++6:i386 libc6:i386
}

function function_unix()
{
    #Libxcb
    sudo apt-get install -y -qq "^libxcb.*" libx11-xcb-dev libxrender-dev libxi-dev

    #安装qt依赖库
    # install needed Qt, OpenAL, opus, qrencode, GTK tray deps, sqlcipher
    sudo apt-get install -y -qq build-essential \
        mesa-common-dev \
        libglu1-mesa-dev  \
        libasound2-dev \
        libgstreamer0.10-dev \
        libgstreamer-plugins-base0.10-dev
        
    #安装第三方库
    if [ "true" != "$RABBITIM_BUILD_THIRDLIBRARY" ]; then
		sudo apt-get install -y -qq libspeex-dev libopus-dev libtheora-dev libvpx-dev libx264-dev \
            libspeex-dev libspeexdsp-dev 
		sudo apt-get install -y -qq libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavfilter-dev #libswresample-dev -qq 
		sudo apt-get install -y -qq libopencv-dev
		sudo apt-get install -y -qq libssl-dev
		sudo apt-get install -y -qq libcurl4-openssl-dev
		sudo apt-get install -y -qq libicu-dev    
    fi

    #安装工具
    sudo apt-get install -y -qq dh-make debhelper
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
	*) #使用circleci测试  
		function_android
		function_unix
		function_mingw
		;;
esac
