#参数:
#    $1:源码的位置 


#运行本脚本前,先运行 build_unix_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   PREFIX=`pwd`/../unix  #修改这里为安装前缀
if [ -z "${PREFIX}" ]; then
    echo "build_unix_envsetup.sh"
    source build_unix_envsetup.sh
fi

if [ -n "$1" ]; then
    SOURCE_CODE=$1
else
    SOURCE_CODE=${PREFIX}/../src/speex
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone http://git.xiph.org/speex.git  ${SOURCE_CODE}"
    git clone http://git.xiph.org/speex.git  ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo ""

if [ ! -f configure ]; then
    echo "sourc autogen.sh"
    source autogen.sh 
fi

mkdir -p build_unix
cd build_unix
rm -fr *

echo "configure ..."

../configure  --prefix=$PREFIX \
    --enable-shared \
    --disable-static \
    --disable-oggtest \
    --enable-vbr \
    SPEEXDSP_CFLAGS=-I$PREFIX/include SPEEXDSP_LIBS=-L$PREFIX/lib

echo "make install"
make -j 2
make install

cd $CUR_DIR
