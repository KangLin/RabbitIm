#参数:
#    $1:源码的位置 


#运行本脚本前,先运行 build_windows_mingw_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   PREFIX=`pwd`/../windows  #修改这里为安装前缀
if [ -z "${PREFIX}" ]; then
    echo "build_unix_envsetup.sh"
    source build_unix_envsetup.sh
fi

if [ -n "$1" ]; then
    SOURCE_CODE=$1
else
    SOURCE_CODE=${PREFIX}/../src/x264
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone git://git.videolan.org/x264.git ${SOURCE_CODE}"
    git clone git://git.videolan.org/x264.git ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo ""

make clean
echo "configure ..."
./configure --prefix=$PREFIX \
    --enable-static \
    --disable-shared \
    --disable-cli \
    --disable-opencl 

echo "make install ..."
make install

cd $CUR_DIR
