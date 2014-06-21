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
    SOURCE_CODE=${PREFIX}/../src/opencv
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone git://github.com/Itseez/opencv.git  ${SOURCE_CODE}"
    git clone git://github.com/Itseez/opencv.git  ${SOURCE_CODE}
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

mkdir -p build_android
cd build_android
rm -fr *

echo "configure ..."
cmake \
    -G"Unix Makefiles" \
    -DCMAKE_TOOLCHAIN_FILE=../platforms/android/android.toolchain.cmake \
    -DANDROID_ABI="armeabi-v7a with NEON" \
    -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-${TOOLCHAIN_VERSION} \
    -DANDROID_NATIVE_API_LEVEL=android-${PLATFORMS_VERSION} \
    -DCMAKE_INSTALL_PREFIX=${PREFIX} \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_DOCS=OFF \
    -DBUILD_opencv_apps=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_ANDROID_EXAMPLES=OFF \
    -DBUILD_TESTS=OFF \
    -DBUILD_FAT_JAVA_LIB=OFF \
    -DBUILD_JASPER=OFF \
    -DBUILD_OPENEXR=OFF \
    -DBUILD_PERF_TESTS=OFF \
    -DBUILD_PACKAGE=OFF \
    -DBUILD_PNG=OFF \
    -DBUILD_TBB=OFF \
    -DBUILD_TIFF=OFF \
    -DBUILD_WITH_DEBUG_INFO=OFF \
    -DWITH_OPENCL=OFF \
    -DBUILD_opencv_video=OFF \
    -DBUILD_opencv_videostab=OFF \
    -DBUILD_opencv_ts=OFF \
    -DBUILD_opencv_java=OFF \
    -DWITH_WEBP=OFF \
    -DWITH_TIFF=OFF \
    -DWITH_PNG=OFF \
    -DWITH_OPENEXR=OFF \
    -DBUILD_opencv_androidcamera=OFF \
    .. 

cmake --build . --target install --config Release

cp -fr ${PREFIX}/sdk/native/jni/include/opencv* ${PREFIX}/include/.
cp -fr ${PREFIX}/sdk/native/libs/armeabi-v7a/* ${PREFIX}/lib/.
rm -fr ${PREFIX}/sdk ${PREFIX}/apk

cd $CUR_DIR
