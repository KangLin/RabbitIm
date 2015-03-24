#!/bin/sh

#作者：康林

#参数:
#    $1:编译目标
#    $2:源码的位置 


#运行本脚本前,先运行 build_unix_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix、unix_mingw）
#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_SOURCE_CODE    #源码目录
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

set -e
HELP_STRING="Usage $0 PLATFORM (android|windows_msvc|windows_mingw|unix|unix_mingw) [SOURCE_CODE_ROOT_DIRECTORY]"

case $1 in
    android|windows_msvc|windows_mingw|unix|unix_mingw)
    RABBITIM_BUILD_TARGERT=$1
    ;;
    *)
    echo "${HELP_STRING}"
    return 1
    ;;
esac

if [ -z "${RABBITIM_BUILD_PREFIX}" ]; then
    echo "build_${RABBITIM_BUILD_TARGERT}_envsetup.sh"
    source build_${RABBITIM_BUILD_TARGERT}_envsetup.sh
fi

if [ -n "$2" ]; then
    echo "start: $0 $1 $2."
    source build_speexdsp.sh ${RABBITIM_BUILD_TARGERT}  $2/speexdsp && \
    source build_speex.sh ${RABBITIM_BUILD_TARGERT} $2/speex && \
    source build_x264.sh ${RABBITIM_BUILD_TARGERT} $2/x264 && \
    source build_libvpx.sh ${RABBITIM_BUILD_TARGERT} $2/libvpx && \
    source build_libyuv.sh ${RABBITIM_BUILD_TARGERT} $2/libyuv && \
    source build_opencv.sh ${RABBITIM_BUILD_TARGERT} $2/opencv && \
    source build_ffmpeg.sh ${RABBITIM_BUILD_TARGERT} $2/ffmpeg && \
    source build_openssl.sh ${RABBITIM_BUILD_TARGERT} $2/openssl && \
    source build_libcurl.sh ${RABBITIM_BUILD_TARGERT} $2/curl && \
    source build_icu.sh ${RABBITIM_BUILD_TARGERT} $2/icu && \
    source build_qt.sh ${RABBITIM_BUILD_TARGERT} $2/qt5 && \
    source build_qxmpp.sh ${RABBITIM_BUILD_TARGERT} $2/qxmpp
else
    echo "start:\n $0 $1 $2."
    #source build_speexdsp.sh ${RABBITIM_BUILD_TARGERT} && \
    #source build_speex.sh ${RABBITIM_BUILD_TARGERT} && \
    source build_x264.sh ${RABBITIM_BUILD_TARGERT} && \
    source build_libvpx.sh ${RABBITIM_BUILD_TARGERT} && \
    source build_libyuv.sh ${RABBITIM_BUILD_TARGERT} && \
    #source build_opencv.sh ${RABBITIM_BUILD_TARGERT} && \
    source build_ffmpeg.sh ${RABBITIM_BUILD_TARGERT} && \
    source build_openssl.sh ${RABBITIM_BUILD_TARGERT} && \
    source build_libcurl.sh ${RABBITIM_BUILD_TARGERT} && \
    source build_icu.sh ${RABBITIM_BUILD_TARGERT} && \
    source build_qt.sh ${RABBITIM_BUILD_TARGERT} && \
    source build_qxmpp.sh ${RABBITIM_BUILD_TARGERT}
fi
