#参数:
#    $1:源码的位置 

#注意：运行前需要设置 msvc 工具链
#   msvc 工具链的环境变量可用下面方法设置：  
#   先从菜单栏中起动vs2013编译命令行工具：  
#   C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts  
#   VS2013 x86 本机工具命令提示  
#   在命令行下

#运行本脚本前,先运行 build_windows_msvc_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   PREFIX=`pwd`/../windows  #修改这里为安装前缀
if [ -z "${PREFIX}" ]; then
    echo "build_windows_msvc_envsetup.sh"
    source build_windows_msvc_envsetup.sh
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

#mkdir -p build_windows_msvc
#cd build_windows_msvc
#rm -fr *

echo "configure ..."
#cmake .. \
#    -DCMAKE_INSTALL_PREFIX="$PREFIX" \
#    -DCMAKE_BUILD_TYPE="Release" \
#    -DBUILD_CURL_TESTS=OFF \
#    -DCURL_STATICLIB=ON
#cmake --build . --target install --config Release
buildconf.bat
cd winbuild
nmake /f Makefile.vc mode=static VC=12 WITH_SSL="$PREFIX"
cd ../builds/libcurl-vc12-x86-release-static-ipv6-sspi
xcopy * "$PREFIX"/.

cd $CUR_DIR
