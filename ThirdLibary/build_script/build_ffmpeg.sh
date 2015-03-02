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

HELP_STRING="Usage $0 PLATFORM (android|windows_msvc|windows_mingw|unix) SOURCE_CODE_ROOT"

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
    echo "build_${RABBITIM_BUILD_TARGERT}_envsetup.sh"
    source build_${RABBITIM_BUILD_TARGERT}_envsetup.sh
fi

if [ -n "$2" ]; then
    RABBITIM_BUILD_SOURCE_CODE=$2
else
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/ffmpeg
fi

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    echo "git clone git://source.ffmpeg.org/ffmpeg.git ${RABBITIM_BUILD_SOURCE_CODE}"
    git clone git://source.ffmpeg.org/ffmpeg.git ${RABBITIM_BUILD_SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${RABBITIM_BUILD_SOURCE_CODE}

echo ""
echo "RABBITIM_BUILD_TARGERT:${RABBITIM_BUILD_TARGERT}"
echo "RABBITIM_BUILD_SOURCE_CODE:$RABBITIM_BUILD_SOURCE_CODE"
echo "CUR_DIR:`pwd`"
echo "RABBITIM_BUILD_PREFIX:$RABBITIM_BUILD_PREFIX"
echo "RABBITIM_BUILD_CROSS_PREFIX:$RABBITIM_BUILD_CROSS_PREFIX"
echo "RABBITIM_BUILD_CROSS_SYSROOT:$RABBITIM_BUILD_CROSS_SYSROOT"
echo ""

git clean -xdf
echo "configure ..."
CONFIG_PARA="--disable-static --enable-shared"
case ${RABBITIM_BUILD_TARGERT} in
    android)
        CONFIG_PARA="--enable-cross-compile --target-os=linux --arch=arm --cpu=armv7-a  --enable-runtime-cpudetect --enable-neon"
        CONFIG_PARA="${CONFIG_PARA} --enable-static --disable-shared --disable-w32threads"
        CONFIG_PARA="${CONFIG_PARA} --cross-prefix=${RABBITIM_BUILD_CROSS_PREFIX}"
        CONFIG_PARA="${CONFIG_PARA} --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        CFLAGS="-march=armv7-a -mfpu=neon -I${RABBITIM_BUILD_PREFIX}/include " \
        LDFLAGS="-L${RABBITIM_BUILD_PREFIX}/lib -lcpu-features" \
    ;;
    unix)
        CFLAGS=" -I$PREFIX/include" \
        LDFLAGS=" -L$PREFIX/lib" 
    ;;
    windows_msvc)
    ;;
    windows_mingw)
    ;;
    *)
    echo "${HELP_STRING}"
    return 2
    ;;
esac

CONFIG_PARA="${CONFIG_PARA} --prefix=$RABBITIM_BUILD_PREFIX --enable-gpl --enable-pic --disable-doc --disable-htmlpages"
CONFIG_PARA="${CONFIG_PARA} --disable-manpages --disable-podpages --disable-txtpages --disable-programs --disable-ffprobe"
CONFIG_PARA="${CONFIG_PARA} --disable-ffserver --disable-ffplay --disable-avdevice"
CONFIG_PARA="${CONFIG_PARA}  --enable-libx264"

echo "./configure ${CONFIG_PARA}  --extra-cflags=\"${CFLAGS}\" --extra-ldflags=\"${LDFLAGS}\""
./configure ${CONFIG_PARA} --extra-cflags="${CFLAGS}" --extra-ldflags="${LDFLAGS}"

echo "make install"
make -j 2
make install

cd $CUR_DIR
