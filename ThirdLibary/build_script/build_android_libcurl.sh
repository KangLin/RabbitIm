#参数:
#    $1:源码的位置 

#运行本脚本前,先运行 build_android_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   PREFIX=`pwd`/../windows  #修改这里为安装前缀
if [ -z "${PREFIX}" ]; then
    echo "build_android_envsetup.sh"
    source build_android_envsetup.sh
fi

if [ -n "$1" ]; then
    SOURCE_CODE=$1
else
    SOURCE_CODE=${PREFIX}/../src/libcurl
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone git://github.com/bagder/curl.git"
    git clone git://github.com/bagder/curl.git  ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo ""

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
    -DCMAKE_MAKE_PROGRAM="$ANDROID_NDK/prebuilt/${HOST}/bin/make" \
    -DCMAKE_TOOLCHAIN_FILE=$PREFIX/../../platforms/android/android.toolchain.cmake \
    -DANDROID_ABI="armeabi-v7a with NEON" \
    -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-${TOOLCHAIN_VERSION} \
    -DANDROID_NATIVE_API_LEVEL=android-${PLATFORMS_VERSION} \
    -DCMAKE_INSTALL_PREFIX=${PREFIX} \
    -DCMAKE_BUILD_TYPE="Release" \
    -DBUILD_CURL_EXE=OFF \
    -DBUILD_CURL_TESTS=OFF \
    -DCURL_DISABLE_LDAP=ON \
    -DCURL_DISABLE_LDAPS=ON \
    -DCMAKE_USE_LIBSSH2=OFF \
    -DCURL_STATICLIB=ON 
    
cmake --build . --target install --config Release

cd $CUR_DIR
