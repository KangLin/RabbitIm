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
echo "make clean"
make clean

echo "configure ......"
./configure  \
    --prefix=$PREFIX \
    --enable-gpl \
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
    --disable-avdevice \
    --enable-static \
    --disable-shared \
    --enable-libx264 \
    --enable-libvpx \
    --extra-cflags=" -I$PREFIX/include" \
    --extra-ldflags=" -L$PREFIX/lib" 
    #--enable-libspeex \
    #--extra-cflags="-static -I$PREFIX/include" \
    #--extra-ldflags="-static -L$PREFIX/lib" 

make clean
make install

cd $CUR_DIR
