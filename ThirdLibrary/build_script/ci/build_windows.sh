#!/bin/bash
set -ev

if [  "$BUILD_TARGERT" = "windows_mingw" ]; then
    export PATH=/C/Qt/Tools/mingw${toolchain_version}_32/bin:$PATH
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
   wget -q -c -O ${SCRIPT_DIR}/../${BUILD_TARGERT}.zip ${DOWNLOAD_FILE}
   unzip -q ${SCRIPT_DIR}/../${BUILD_TARGERT}.zip -d ${SCRIPT_DIR}/../${BUILD_TARGERT}
fi

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${SCRIPT_DIR}/../Tools/android-sdk
    export ANDROID_NDK_ROOT=${SCRIPT_DIR}/../Tools/android-ndk
    export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    export QT_ROOT=${SCRIPT_DIR}/../Tools/Qt/${QT_VERSION}/${QT_VERSION_DIR}/android_armv7
    if [ "${QT_VERSION}" = "5.2.1" ]; then
        export QT_ROOT=${SCRIPT_DIR}/../Tools/Qt/${QT_VERSION}/android_armv7
    fi
    export PATH=${SCRIPT_DIR}/../Tools/apache-ant/bin:$JAVA_HOME:$PATH
fi
if [ "$BUILD_TARGERT" = "windows_mingw" ]; then
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
        ./build_$v.sh ${BUILD_TARGERT}
    else
        ./build_$v.sh ${BUILD_TARGERT} ${SOURCE_DIR}/$v #> /dev/null
    fi
done
#./build_webrtc.sh ${BUILD_TARGERT}

#./build_openssl.sh ${BUILD_TARGERT} ${SOURCE_DIR}/openssl > /dev/null
#./build_libcurl.sh ${BUILD_TARGERT} ${SOURCE_DIR}/curl #> /dev/null
#./build_libvpx.sh ${BUILD_TARGERT} ${SOURCE_DIR}/libvpx > /dev/null
#./build_libyuv.sh ${BUILD_TARGERT} ${SOURCE_DIR}/libyuv > /dev/null
#./build_x264.sh ${BUILD_TARGERT} ${SOURCE_DIR}/libx264 > /dev/null
#./build_ffmpeg.sh ${BUILD_TARGERT} ${SOURCE_DIR}/ffmpeg # > /dev/null
#
#./build_qxmpp.sh ${BUILD_TARGERT} ${SOURCE_DIR}/qxmpp > /dev/null
#./build_qzxing.sh ${BUILD_TARGERT} ${SOURCE_DIR}/qzxing > /dev/null
#./build_rabbitim.sh ${BUILD_TARGERT} ${SOURCE_DIR} qmake
