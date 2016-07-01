#!/bin/bash
set -ev

SOURCE_DIR=../../..
if [ -n "$1" ]; then
    SOURCE_DIR=$1
fi
if [ -z "$QT_VERSION_DIR" ]; then
    QT_VERSION_DIR="5.6"
fi
if [ -z "${QT_VERSION}" ]; then
    QT_VERSION="5.6.0"
fi

sudo apt-get update -qq
sudo apt-get install -y -qq subversion git wget ant
sudo apt-get install -y build-essential \
        libtool \
        autotools-dev \
        automake \
        autoconf \
        checkinstall \
        check \
        m4

#文档产生工具
sudo apt-get install -y -qq doxygen 
#编译qt webkit 需要
sudo apt-get install -y -qq ruby perl python bison flex gperf
#icu工具
sudo apt-get install -y -qq icu-devtools

mkdir -p ${SOURCE_DIR}/ThirdLibrary/Tools
cd ${SOURCE_DIR}/ThirdLibrary/Tools

#Download android ndk
if [ ! -d "`pwd`/android-ndk" ]; then
    wget -q -c http://dl.google.com/android/ndk/android-ndk-r10e-linux-x86_64.bin
    chmod u+x android-ndk-r10e-linux-x86_64.bin
    ./android-ndk-r10e-linux-x86_64.bin > /dev/null
    mv android-ndk-r10e android-ndk
    rm android-ndk-r10e-linux-x86_64.bin
fi
export ANDROID_NDK_ROOT=`pwd`/android-ndk
export ANDROID_NDK=$ANDROID_NDK_ROOT

#Download android sdk
if [ ! -d "`pwd`/android-sdk" ]; then
    wget -q -c https://dl.google.com/android/android-sdk_r24.4.1-linux.tgz
    tar xf android-sdk_r24.4.1-linux.tgz 
    mv android-sdk-linux android-sdk
    rm android-sdk_r24.4.1-linux.tgz 
    (sleep 5 ; while true ; do sleep 1 ; printf 'y\r\n' ; done ) \
    | android-sdk/tools/android update sdk -u -t build-tools-20.0.0,android-18,extra  #platform,platform-tool,tool 
fi
export ANDROID_SDK_ROOT=`pwd`/android-sdk
export ANDROID_SDK=$ANDROID_SDK_ROOT

#Download cmake
if [ ! -d "`pwd`/cmake" ]; then
    wget -q -c --no-check-certificate http://www.cmake.org/files/v3.1/cmake-3.1.0-Linux-x86_64.tar.gz
    tar xf cmake-3.1.0-Linux-x86_64.tar.gz
    mv cmake-3.1.0-Linux-x86_64 cmake
    rm cmake-3.1.0-Linux-x86_64.tar.gz
fi
export PATH=`pwd`/cmake/bin:$PATH

#Download qt
QT_DIR=`pwd`/Qt/${QT_VERSION}
if [ ! -d "${QT_DIR}" ]; then
    wget -c --no-check-certificate -nv http://download.qt.io/official_releases/qt/${QT_VERSION_DIR}/${QT_VERSION}/qt-opensource-linux-x64-android-${QT_VERSION}.run
    bash ${SOURCE_DIR}/ThirdLibrary/build_script/ci/qt-installer.sh qt-opensource-linux-x64-android-${QT_VERSION}.run ${QT_DIR}
    rm qt-opensource-linux-x64-android-${QT_VERSION}.run
fi
export QT_ROOT=${QT_DIR}/${QT_VERSION_DIR}/android_armv7
if [ "${QT_VERSION}" = "5.2.1" ]; then
    export QT_ROOT=${QT_DIR}/${QT_VERSION}/android_armv7
fi

RABBITIM_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
if [ "$RABBITIM_MAKE_JOB_PARA" = "-j1" ];then
        RABBITIM_MAKE_JOB_PARA="-j2"
fi
export RABBITIM_MAKE_JOB_PARA
export RABBITIM_USE_REPOSITORIES="FALSE"

cd ${SOURCE_DIR}/ThirdLibrary/build_script
./build_webrtc.sh android
./build_openssl.sh android > /dev/null
./build_libcurl.sh android
./build_qrencode.sh android
./build_x264.sh android
./build_libvpx.sh android
./build_libyuv.sh android
./build_ffmpeg.sh android
./build_rabbitim.sh android ${SOURCE_DIR} qmake
