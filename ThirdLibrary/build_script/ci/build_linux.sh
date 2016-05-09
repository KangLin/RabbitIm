#!/bin/bash
set -e

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

#sudo add-apt-repository -y ppa:beineri/opt-qt532-trusty
sudo apt-get update -qq
#sudo apt-get install -y -qq subversion git wget ant
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

# 依赖库  
sudo apt-get install -y -qq libvpx-dev \
    libavcodec-dev libavformat-dev libavutil-dev libswscale-dev \
    libqrencode-dev \
    libssl-dev \
    libcurl4-openssl-dev
sudo apt-get install -y -qq "^libxcb.*" libx11-xcb-dev libxrender-dev libxi-dev
#QT 需要===============================
#编译qt webkit 需要
sudo apt-get install -y -qq bison flex gperf ruby 
#icu工具
sudo apt-get install -y -qq icu-devtools libicu-dev 
#opengl
sudo apt-get install -y -qq mesa-common-dev libglu1-mesa-dev
# libasound2、libgstreamer
sudo apt-get install -y -qq libasound2-dev
sudo apt-get install -y -qq libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev
#QT 需要===============================

mkdir -p ${SOURCE_DIR}/ThirdLibrary/Tools
cd ${SOURCE_DIR}/ThirdLibrary/Tools

# cmake
if [ ! -d "`pwd`/cmake" ]; then
    wget -q -c --no-check-certificate http://www.cmake.org/files/v3.1/cmake-3.1.0-Linux-x86_64.tar.gz
    tar xf cmake-3.1.0-Linux-x86_64.tar.gz
    mv cmake-3.1.0-Linux-x86_64 cmake
    rm cmake-3.1.0-Linux-x86_64.tar.gz
fi
export PATH=`pwd`/cmake/bin:$PATH

# Qt qt安装参见：https://github.com/benlau/qtci  
QT_DIR=`pwd`/Qt/${QT_VERSION}
if [ ! -d "${QT_DIR}" ]; then
    wget -c --no-check-certificate -nv http://download.qt.io/official_releases/qt/${QT_VERSION_DIR}/${QT_VERSION}/qt-opensource-linux-x64-android-${QT_VERSION}.run
    bash ${SOURCE_DIR}/ThirdLibrary/build_script/ci/qt-installer.sh qt-opensource-linux-x64-android-${QT_VERSION}.run ${QT_DIR}
    rm qt-opensource-linux-x64-android-${QT_VERSION}.run
fi
export QT_ROOT=${QT_DIR}/${QT_VERSION_DIR}/gcc_64
if [ "${QT_VERSION}" = "5.2.1" ]; then
    export QT_ROOT=${QT_DIR}/${QT_VERSION}/gcc_64
fi

RABBITIM_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
if [ "$RABBITIM_MAKE_JOB_PARA" = "-j1" ];then
        RABBITIM_MAKE_JOB_PARA="-j2"
fi
export RABBITIM_MAKE_JOB_PARA
export RABBITIM_USE_REPOSITORIES="FALSE"

cd ${SOURCE_DIR}/ThirdLibrary/build_script
mkdir ${SOURCE_DIR}/ThirdLibrary/src
#./build_webrtc.sh unix
./build_qxmpp.sh unix > /dev/null
./build_qzxing.sh unix > /dev/null

./build_rabbitim.sh unix ${SOURCE_DIR} qmake #> /dev/null
