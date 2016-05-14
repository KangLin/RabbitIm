#!/bin/bash
set -ev

SOURCE_DIR=../../..
if [ -n "$1" ]; then
    SOURCE_DIR=$1
fi

cd ${SOURCE_DIR}/ThirdLibrary/build_script
SOURCE_DIR=`pwd`

if [ "$AUTOBUILD_COMPLER" = "MinGW" ]; then
    TARGET=windows_mingw 
    RABBITIM_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
    if [ "$RABBITIM_MAKE_JOB_PARA" = "-j1" ];then
            RABBITIM_MAKE_JOB_PARA="-j2"
    fi
    export RABBITIM_MAKE_JOB_PARA
else 
    TARGET=windows_msvc
fi

export RABBITIM_USE_REPOSITORIES="FALSE"
#./build_webrtc.sh ${TARGET}

./build_openssl.sh ${TARGET} ${SOURCE_DIR}/ThirdLibrary/src/openssl > /dev/null
./build_libcurl.sh ${TARGET} ${SOURCE_DIR}/ThirdLibrary/src/curl > /dev/null
./build_libvpx.sh ${TARGET} ${SOURCE_DIR}/ThirdLibrary/src/libvpx > /dev/null
./build_libyuv.sh ${TARGET} ${SOURCE_DIR}/ThirdLibrary/src/libyuv > /dev/null
  
./build_ffmpeg.sh ${TARGET} ${SOURCE_DIR}/ThirdLibrary/src/ffmpeg > /dev/null
   
./build_qxmpp.sh ${TARGET} ${SOURCE_DIR}/ThirdLibrary/src/qxmpp > /dev/null
./build_qzxing.sh ${TARGET} ${SOURCE_DIR}/ThirdLibrary/src/qzxing > /dev/null
./build_rabbitim.sh ${TARGET} ${SOURCE_DIR} qmake
    
