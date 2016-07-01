#!/bin/bash

#bash用法：
#   在用一sh进程中执行脚本script.sh:
#   source script.sh
#   . script.sh
#   注意这种用法，script.sh开头一行不能包含 #!/bin/bash

#   新建一个sh进程执行脚本script.sh:
#   sh script.sh
#   ./script.sh
#   注意这种用法，script.sh开头一行必须包含 #!/bin/bash

#作者：康林

#参数:
#    $1:编译目标
#    $2:源码的位置 

#运行本脚本前,先运行 build_unix_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix）
#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_SOURCE_CODE    #源码目录
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

set -e
HELP_STRING="Usage $0 PLATFORM (android|windows_msvc|windows_mingw|unix) [SOURCE_CODE_ROOT_DIRECTORY]"

case $1 in
    android|windows_msvc|windows_mingw|unix)
    RABBITIM_BUILD_TARGERT=$1
    ;;
    *)
    echo "${HELP_STRING}"
    return 1
    ;;
esac

if [ -z "${RABBITIM_BUILD_PREFIX}" ]; then
    echo ". `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh"
    . `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh
fi

#产生修改前缀脚本
./change_prefix.sh

if [ -n "$2" ]; then
    #echo "Source dir:$2"
    ./build_openssl.sh ${RABBITIM_BUILD_TARGERT} $2/openssl && \
    ./build_libcurl.sh ${RABBITIM_BUILD_TARGERT} $2/curl && \
    ./build_qrencode.sh ${RABBITIM_BUILD_TARGERT} $2/libqrencode && \
    ./build_x264.sh ${RABBITIM_BUILD_TARGERT} $2/x264 && \
    ./build_libvpx.sh ${RABBITIM_BUILD_TARGERT} $2/libvpx && \
    ./build_libyuv.sh ${RABBITIM_BUILD_TARGERT} $2/libyuv && \
    ./build_ffmpeg.sh ${RABBITIM_BUILD_TARGERT} $2/ffmpeg && \
    ./build_qt.sh ${RABBITIM_BUILD_TARGERT} $2/qt5 && \
    ./build_qxmpp.sh ${RABBITIM_BUILD_TARGERT} $2/qxmpp && \
    ./build_qzxing.sh ${RABBITIM_BUILD_TARGERT} $2/qzxing
else
    ./build_openssl.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_libcurl.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_qrencode.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_x264.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_libvpx.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_libyuv.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_ffmpeg.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_qt.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_qxmpp.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_qzxing.sh ${RABBITIM_BUILD_TARGERT} 
fi


exit 0


if [ -n "$2" ]; then
    echo "Source dir:$2"
    if [ "${RABBITIM_BUILD_TARGERT}" != "windows_msvc" ]; then
        echo "building ......"
        ./build_libopus.sh ${RABBITIM_BUILD_TARGERT} $2/libopus && \
        ./build_speexdsp.sh ${RABBITIM_BUILD_TARGERT} $2/speexdsp && \
        ./build_speex.sh ${RABBITIM_BUILD_TARGERT} $2/speex && \
        ./build_libsodium.sh ${RABBITIM_BUILD_TARGERT} $2/libsodium && \
        ./build_filter_audio.sh ${RABBITIM_BUILD_TARGERT} $2/filter_audio && \
        ./build_toxcore.sh ${RABBITIM_BUILD_TARGERT} $2/toxcore
    fi
    ./build_opencv.sh ${RABBITIM_BUILD_TARGERT} $2/opencv && \
    # ./build_pjsip.sh ${RABBITIM_BUILD_TARGERT} $2/pjsip && \
    # ./build_icu.sh ${RABBITIM_BUILD_TARGERT} $2/icu && \
    ./build_gdal.sh ${RABBITIM_BUILD_TARGERT} $2/gdal && \
    ./build_osg.sh ${RABBITIM_BUILD_TARGERT} $2/osg && \
    ./build_osgearth.sh ${RABBITIM_BUILD_TARGERT} $2/osgearth
else
    if [ "${RABBITIM_BUILD_TARGERT}" != "windows_msvc" ]; then
        ./build_speexdsp.sh ${RABBITIM_BUILD_TARGERT} && \
        ./build_speex.sh ${RABBITIM_BUILD_TARGERT} && \
        ./build_libopus.sh ${RABBITIM_BUILD_TARGERT} && \
        ./build_libsodium.sh ${RABBITIM_BUILD_TARGERT} && \
        ./build_toxcore.sh ${RABBITIM_BUILD_TARGERT}
    fi
    ./build_opencv.sh ${RABBITIM_BUILD_TARGERT} && \
    # ./build_pjsip.sh ${RABBITIM_BUILD_TARGERT} && \
    # ./build_icu.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_gdal.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_osg.sh ${RABBITIM_BUILD_TARGERT} && \
    ./build_osgearth.sh ${RABBITIM_BUILD_TARGERT}
fi
