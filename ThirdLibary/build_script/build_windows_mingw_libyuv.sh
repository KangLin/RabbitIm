#参数:
#    $1:源码的位置 

#运行本脚本前,先运行 build_windows_mingw_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   PREFIX=`pwd`/../windows  #修改这里为安装前缀
if [ -z "${PREFIX}" ]; then
    echo "build_windows_mingw_envsetup.sh"
    source build_windows_mingw_envsetup.sh
fi

if [ -n "$1" ]; then
    SOURCE_CODE=$1
else
    SOURCE_CODE=${PREFIX}/../src/libyuv
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone http://git.chromium.org/external/libyuv.git"
    git clone http://git.chromium.org/external/libyuv.git  ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo ""

mkdir -p build_windows_mingw
cd build_windows_mingw
rm -fr *

echo "configure ..."
cmake .. \
    -G"Unix Makefiles" \
    -DCMAKE_INSTALL_PREFIX="$PREFIX" \
    -DCMAKE_BUILD_TYPE="Release" 
cmake --build . --target install --config Release

cd $CUR_DIR
