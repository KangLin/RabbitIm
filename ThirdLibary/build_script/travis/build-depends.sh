#!/bin/sh
set -e

function function_common()
{
    sudo apt-get install autoconf automake libtool m4 -qq

    #编译qt webkit 需要
    sudo apt-get install bison flex gperf ruby #icu-devtools 

    wget http://mirrors.kernel.org/ubuntu/pool/main/i/icu/libicu52_52.1-3_amd64.deb
    sudo dpkg -i libicu52_52.1-3_amd64.deb #icu库,用于 Unicode and Globalization 支持
}

function function_android()
{
    function_common
    sudo apt-get install zlib1g:i386 libstdc++6:i386 libc6:i386 -y -qq
}

function function_unix()
{
    function_common
    
    #安装qt依赖库
    sudo apt-get install build-essential -y -qq
    sudo apt-get install mesa-common-dev -y -qq
    sudo apt-get install libglu1-mesa-dev -y -qq
    sudo apt-get install libasound2-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev -y -qq
    
    #sudo apt-get install -y qtbase5-dev qtmultimedia5-dev qttools5-dev-tools -qq
    sudo apt-get install -y libspeex-dev libtheora-dev libvpx-dev -qq
    sudo apt-get install -y libx264-dev -qq
    sudo apt-get install libspeex-dev libspeexdsp-dev -qq
    sudo apt-get install -y libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavfilter-dev -qq
    sudo apt-get install -y libopencv-dev -qq
    #sudo apt-get install -y libqxmpp-dev -qq
    sudo apt-get install -y libssl-dev -qq
    sudo apt-get install -y libcurl4-openssl-dev 

    sudo apt-get install libicu-dev
    
    sudo apt-get install -y yasm -qq
    sudo apt-get install -y doxygen -qq
}

function function_mingw()
{
    function_common

    #sudo apt-get install mingw-w64 mingw-w64-i686-dev 
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
    unix_mingw)
        function_mingw
        ;;
    *)
        ;;
esac