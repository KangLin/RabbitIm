#参数:
#    $1:源码的位置 

#运行本脚本前,先运行 build_android_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   ANDROID_NDK_ROOT=       #指定android ndk根目录
#   TOOLCHAIN_VERSION=4.8 #工具链版本号
#   PLATFORMS_VERSION=18  #android api (平台)版本号
#   PREBUILT=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-${TOOLCHAIN_VERSION}/prebuilt
#   PLATFORM=$ANDROID_NDK_ROOT/platforms/android-${PLATFORMS_VERSION}/arch-arm
#   CROSS_PREFIX=$PREBUILT/${HOST}/bin/arm-linux-androideabi-
#   PREFIX=`pwd`/../android
if [ -z "${ANDROID_NDK_ROOT}" -o  -z "${PREBUILT}" -o -z "${PLATFORM}" -o -z "${CROSS_PREFIX}" ]; then
    echo "source build_android_envsetup.sh"
    source build_android_envsetup.sh
fi

if [ -n "$1" ]; then
    SOURCE_CODE=$1
else
    SOURCE_CODE=${PREFIX}/../src/speexdsp
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone http://git.xiph.org/speexdsp.git  ${SOURCE_CODE}"
    git clone http://git.xiph.org/speexdsp.git  ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "ANDROID_NDK_ROOT:$ANDROID_NDK_ROOT"
echo "ANDROID_NDK:$ANDROID_NDK_ROOT"
echo "JAVA_HOME:$JAVA_HOME"
echo "PREFIX:$PREFIX"
echo "PREBUILT:$PREBUILT"
echo "PLATFORM:$PLATFORM"
echo "CROSS_PREFIX:$CROSS_PREFIX"
echo ""

if [ ! -f configure ]; then
    echo "./autogen.sh"
    ./autogen.sh 
fi

mkdir -p build_android
cd build_android

echo "configure ..."
export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
../configure --prefix=$PREFIX  \
    CC=${CROSS_PREFIX}gcc \
    --disable-shared \
    --enable-static \
    CFLAGS="-march=armv7-a -mfpu=neon --sysroot=${PLATFORM}" \
    CPPFLAGS="-march=armv7-a -mfpu=neon --sysroot=${PLATFORM}" \
    --host=arm-linux-androideabi 

echo "make install"
make clean; make install

cd $CUR_DIR
