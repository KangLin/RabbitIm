#!/bin/sh
set -e

function function_common()
{
    sudo apt-get install autoconf automake libtool m4 -y -qq

    sudo apt-get install subversion git wget ant -y -qq
    #文档产生工具
    sudo apt-get install -y doxygen -qq
    #编译qt webkit 需要
    sudo apt-get install bison flex gperf ruby -y -qq

    #icu工具
    #wget http://mirrors.kernel.org/ubuntu/pool/main/i/icu/libicu52_52.1-3_amd64.deb
    #sudo dpkg -i libicu52_52.1-3_amd64.deb #icu库,用于 Unicode and Globalization 支持
    sudo apt-get install libicu48 -y -qq
}

function function_android()
{
    function_common
    sudo apt-get install zlib1g:i386 libstdc++6:i386 libc6:i386 -y -qq
}

function function_unix()
{
    function_common

    #Libxcb
    sudo apt-get install "^libxcb.*" libx11-xcb-dev libxrender-dev libxi-dev

    #安装qt依赖库
    sudo apt-get install build-essential -y -qq
    sudo apt-get install mesa-common-dev -y -qq
    sudo apt-get install libglu1-mesa-dev -y -qq
    sudo apt-get install libasound2-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev -y -qq
    
    #安装第三方库
    #if [ -z "$BUILD_THIRDLIBARY" ]; then
	#	sudo apt-get install -y libspeex-dev libtheora-dev libvpx-dev -qq
	#	sudo apt-get install -y libx264-dev -qq
	#	sudo apt-get install libspeex-dev libspeexdsp-dev -qq
	#	sudo apt-get install -y libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavfilter-dev -qq
	#	sudo apt-get install -y libopencv-dev -qq
	#	sudo apt-get install -y libssl-dev -qq
	#	sudo apt-get install -y libcurl4-openssl-dev -qq
	#	sudo apt-get install libicu-dev -qq
    #fi

    #安装工具
    sudo apt-get install -y dh-make debhelper -qq
}

function function_mingw()
{
    function_common

	#安装工具
    sudo apt-get install nsis -y -qq
}

#sudo add-apt-repository -y ppa:ubuntu-sdk-team/ppa
#sudo add-apt-repository --yes ppa:kalakris/cmake

sudo apt-get update -y -qq

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
