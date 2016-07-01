#!/bin/bash
set -ev

if [  "$BUILD_TARGET" = "windows_mingw" ]; then
    export PATH=/C/Qt/Tools/mingw${toolchain_version}_32/bin:/usr/bin
fi

SOURCE_DIR=`pwd`
if [ -n "$1" ]; then
    SOURCE_DIR=$1
fi
SCRIPT_DIR=${SOURCE_DIR}/build_script
if [ -d ${SOURCE_DIR}/ThirdLibrary/build_script ]; then
    SCRIPT_DIR=${SOURCE_DIR}/ThirdLibrary/build_script
fi
cd ${SCRIPT_DIR}
SOURCE_DIR=${SCRIPT_DIR}/../src

#下载预编译库
if [ -n "$DOWNLOAD_FILE" ]; then
   wget -q -c -O ${SCRIPT_DIR}/../${BUILD_TARGET}.zip ${DOWNLOAD_FILE}
   unzip -q ${SCRIPT_DIR}/../${BUILD_TARGET}.zip -d ${SCRIPT_DIR}/../${BUILD_TARGET}
fi

if [ "$BUILD_TARGET" = "windows_mingw" ]; then
    RABBITIM_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
    if [ "$RABBITIM_MAKE_JOB_PARA" = "-j1" ];then
            RABBITIM_MAKE_JOB_PARA="-j2"
    fi
    export RABBITIM_MAKE_JOB_PARA
fi
echo "RABBITIM_BUILD_THIRDLIBRARY:$RABBITIM_BUILD_THIRDLIBRARY"
for v in $RABBITIM_BUILD_THIRDLIBRARY
do
    if [ "$v" = "rabbitim" ]; then
        ./build_$v.sh ${BUILD_TARGET}
    else
        ./build_$v.sh ${BUILD_TARGET} ${SOURCE_DIR}/$v #> /dev/null
    fi
done
#./build_webrtc.sh ${BUILD_TARGET}

#./build_openssl.sh ${BUILD_TARGET} ${SOURCE_DIR}/openssl > /dev/null
#./build_libcurl.sh ${BUILD_TARGET} ${SOURCE_DIR}/curl #> /dev/null
#./build_libvpx.sh ${BUILD_TARGET} ${SOURCE_DIR}/libvpx > /dev/null
#./build_libyuv.sh ${BUILD_TARGET} ${SOURCE_DIR}/libyuv > /dev/null
#./build_x264.sh ${BUILD_TARGET} ${SOURCE_DIR}/libx264 > /dev/null
#./build_ffmpeg.sh ${BUILD_TARGET} ${SOURCE_DIR}/ffmpeg # > /dev/null
#
#./build_qxmpp.sh ${BUILD_TARGET} ${SOURCE_DIR}/qxmpp > /dev/null
#./build_qzxing.sh ${BUILD_TARGET} ${SOURCE_DIR}/qzxing > /dev/null
#./build_rabbitim.sh ${BUILD_TARGET} ${SOURCE_DIR} qmake
