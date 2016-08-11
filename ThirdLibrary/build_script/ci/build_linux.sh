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
echo "RABBITIM_BUILD_THIRDLIBRARY:$RABBITIM_BUILD_THIRDLIBRARY"
for v in $RABBITIM_BUILD_THIRDLIBRARY
do
    if [ "$v" = "rabbitim" ]; then
        bash build_$v.sh ${BUILD_TARGERT}
    else
        echo "bash build_$v.sh ${BUILD_TARGERT} ${SOURCE_DIR}/$v"
        bash build_$v.sh ${BUILD_TARGERT} ${SOURCE_DIR}/$v > /dev/null
    fi
done
exit 0

#./build_webrtc.sh unix > /dev/null
if [ "$RABBITIM_BUILD_THIRDLIBRARY" = "TRUE" ]; then
    bash build_libzlib.sh ${BUILD_TARGERT} 
    bash build_minizip.sh ${BUILD_TARGERT} 
    bash build_libpng.sh ${BUILD_TARGERT} 
    bash build_jpeg.sh ${BUILD_TARGERT} 
    bash build_libtiff.sh ${BUILD_TARGERT} 
    #bash build_geos.sh ${BUILD_TARGERT} 
    #bash build_gdal.sh ${BUILD_TARGERT} 
    bash build_openssl.sh ${BUILD_TARGERT} > /dev/null
    bash build_libcurl.sh ${BUILD_TARGERT} > /dev/null
    #bash build_osg.sh ${BUILD_TARGERT} 
    #bash build_osgearth.sh ${BUILD_TARGERT} 
    bash build_qrencode.sh ${BUILD_TARGERT} > /dev/null
    bash build_x264.sh ${BUILD_TARGERT}  > /dev/null
    bash build_libvpx.sh ${BUILD_TARGERT} > /dev/null
    bash build_libyuv.sh ${BUILD_TARGERT} > /dev/null
    bash build_ffmpeg.sh ${BUILD_TARGERT} > /dev/null
fi

bash build_qxmpp.sh ${BUILD_TARGERT}  > /dev/null
bash build_qzxing.sh ${BUILD_TARGERT} > /dev/null
bash build_rabbitim.sh ${BUILD_TARGERT} ${SOURCE_DIR} ${QMAKE} #> /dev/null
