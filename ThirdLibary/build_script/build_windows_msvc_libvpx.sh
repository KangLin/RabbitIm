#参数:
#    $1:源码的位置 

#注意：运行前需要设置 msvc 工具链
#   msvc 工具链的环境变量可用下面方法设置：  
#   先从菜单栏中起动vs2013编译命令行工具：  
#   C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts  
#   VS2013 x86 本机工具命令提示  
#   在命令行下，启动msys。 
#   `c:\MinGW\msys\1.0\msys.bat`  
#   注意，msys中不要装link工具，否则会导致出错。如果有link工具，暂时把它命名成其它名称。  
#   然后再进入脚本目录：`cd ${RabbitImRoot}/ThirdLibary/build_script`。再运行你想运行的编译脚本。例如： `./build_windows_mscv.sh` 
   
#运行本脚本前,先运行 build_windows_msvc_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   PREFIX=`pwd`/../windows  #修改这里为安装前缀
if [ -z "${PREFIX}" ]; then
    echo "source build_windows_msvc_envsetup.sh"
    source build_windows_msvc_envsetup.sh
fi

if [ -n "$1" ]; then
    SOURCE_CODE=$1
else
    SOURCE_CODE=${PREFIX}/../src/libvpx
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone https://chromium.googlesource.com/webm/libvpx  ${SOURCE_CODE}"
    git clone https://chromium.googlesource.com/webm/libvpx  ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo ""

mkdir -p build_windows_msvc
cd build_windows_msvc

rm -fr *

echo "configure ..."
../configure \
    --enable-static-msvcrt \
    --disable-docs \
    --disable-examples  \
    --disable-install-docs \
    --disable-install-bins \
    --enable-install-libs \
    --disable-unit-tests \
    --disable-debug  \
    --disable-debug-libs \
    --target=x86-win32-vs12 \
    --prefix=$PREFIX 
    #--enable-shared \
    #--disable-static 
    

echo "make install"

make
make install

cp ${PREFIX}/lib/Win32/vpxmt.lib ${PREFIX}/lib/vpx.lib
rm -fr ${PREFIX}/lib/Win32

cd $CUR_DIR
