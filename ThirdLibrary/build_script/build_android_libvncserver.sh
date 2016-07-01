#参数:
#    $1:源码的位置 

#运行本脚本前,先运行 build_android_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   ANDROID_NDK=     #指定android ndk根目录
#   HOST                  #主机平台
#   TOOLCHAIN_VERSION=4.8 #工具链版本号
#   PLATFORMS_VERSION=18  #android api (平台)版本号
#   PREBUILT=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-${TOOLCHAIN_VERSION}/prebuilt
#   PLATFORM=$ANDROID_NDK_ROOT/platforms/android-${PLATFORMS_VERSION}/arch-arm
#   CROSS_PREFIX=$PREBUILT/${HOST}/bin/arm-linux-androideabi-
#   PREFIX=`pwd`/../android
if [ -z "$ANDROID_NDK" -o -z "$PREFIX" -o -z "$HOST" ]; then
    echo "source build_android_envsetup.sh"
    source build_android_envsetup.sh
fi

if [ -n "$1" ]; then
    SOURCE_CODE=$1
else
    SOURCE_CODE=${PREFIX}/../src/libvncserver
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "https://github.com/LibVNC/libvncserver.git  ${SOURCE_CODE}"
    git clone https://github.com/LibVNC/libvncserver.git  ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo "PREBUILT:$PREBUILT"
echo "ANDROID_NDK:$ANDROID_NDK"
echo "HOST:${HOST}"
echo ""

if [ ! -f configure ]; then
    echo "autoreconf -fiv"
    autoreconf -fiv
fi

mkdir -p build_android
cd build_android
rm -fr *

echo "configure ..."

../configure  --prefix=$PREFIX \
    --with-sysroot=${PLATFORM} \
    CC=${CROSS_PREFIX}gcc \
    CFLAGS="-march=armv7-a -mfpu=neon --sysroot=${PLATFORM}" \
    CPPFLAGS="-march=armv7-a -mfpu=neon --sysroot=${PLATFORM}" \
    --host=arm-linux-androideabi 

make 

exit 0

mkdir -p build_android
cd build_android
rm -fr *

case `uname -s` in
    MINGW* | CYGWIN*)
        GENERATORS="MinGW Makefiles"
        ;;
    Linux* | Unix* | *)
        GENERATORS="Unix Makefiles" 
        ;;
esac

cmake .. \
    -G"${GENERATORS}"\
    -DCMAKE_TOOLCHAIN_FILE=$PREFIX/../../platforms/android/android.toolchain.cmake \
    -DANDROID_ABI="armeabi-v7a with NEON" \
    -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-${TOOLCHAIN_VERSION} \
    -DANDROID_NATIVE_API_LEVEL=android-${PLATFORMS_VERSION} \
    -DCMAKE_INSTALL_PREFIX=${PREFIX} \
    -DANDROID_SYSROOT=${PLATFORM}

cmake --build . --target install --config Release

cd $CUR_DIR
