#参数:
#    $1:源码的位置 

#注意：运行前需要设置 msvc 工具链
#   msvc 工具链的环境变量可用下面方法设置：  
#   先从菜单栏中起动vs2013编译命令行工具：  
#   C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts  
#   VS2013 x86 本机工具命令提示  
#   在命令行下，启动msys。 
#   `c:\MinGW\msys\1.0\msys`  
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
    SOURCE_CODE=${PREFIX}/../src/ffmpeg
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone git://source.ffmpeg.org/ffmpeg.git"
    git clone git://source.ffmpeg.org/ffmpeg.git ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo ""

echo "configure ..."
./configure  \
        --target-os=win32 \
        --arch=x86 \
        --enable-cross-compile \
        --toolchain=msvc \
        --prefix=$PREFIX \
        --enable-runtime-cpudetect \
        --disable-doc \
        --disable-htmlpages \
        --disable-manpages  \
        --disable-podpages \
        --disable-txtpages \
        --disable-programs \
        --disable-ffprobe \
        --disable-ffserver \
        --disable-ffplay \
        --disable-ffmpeg \
        --disable-avdevice # \
        #--disable-shared \
        #--extra-cflags="-I${PREFIX}/include" \
        #--extra-ldflags="/LIBPATH:${PREFIX}/lib" \
        #--enable-libvpx

echo "make install"
make clean; make install

#mv ${PREFIX}/lib/libavcodec.a ${PREFIX}/lib/avcodec.lib
#mv ${PREFIX}/lib/libavformat.a ${PREFIX}/lib/avformat.lib
#mv ${PREFIX}/lib/libswscale.a ${PREFIX}/lib/swscale.lib
#mv ${PREFIX}/lib/libswresample.a ${PREFIX}/lib/swresample.lib
#mv ${PREFIX}/lib/libavfilter.a ${PREFIX}/lib/avfilter.lib
#mv ${PREFIX}/lib/libavutil.a ${PREFIX}/lib/avutil.lib

cd $CUR_DIR
