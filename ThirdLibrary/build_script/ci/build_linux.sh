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

cd ${SOURCE_DIR}/ThirdLibrary/build_script
mkdir -p ${SOURCE_DIR}/ThirdLibrary/src
#./build_webrtc.sh unix > /dev/null

if [ "$RABBITIM_BUILD_THIRDLIBRARY" = "TRUE" ]; then
    bash build_openssl.sh ${BUILD_TARGERT} > /dev/null
    bash build_libcurl.sh ${BUILD_TARGERT} > /dev/null
    bash build_qrencode.sh ${BUILD_TARGERT} > /dev/null
    bash build_x264.sh ${BUILD_TARGERT}  > /dev/null
    bash build_libvpx.sh ${BUILD_TARGERT} > /dev/null
    bash build_libyuv.sh ${BUILD_TARGERT} > /dev/null
    bash build_ffmpeg.sh ${BUILD_TARGERT} > /dev/null
fi

bash build_qxmpp.sh ${BUILD_TARGERT}  > /dev/null
bash build_qzxing.sh ${BUILD_TARGERT} > /dev/null
bash build_rabbitim.sh ${BUILD_TARGERT} ${SOURCE_DIR} ${QMAKE} #> /dev/null
