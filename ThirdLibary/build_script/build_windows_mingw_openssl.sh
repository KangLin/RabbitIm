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
    SOURCE_CODE=${PREFIX}/../src/openssl
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone https://github.com/openssl/openssl"
    git clone https://github.com/openssl/openssl ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo ""

make clean
echo "configure openssl ..."
./Configure \
    --prefix=${PREFIX} \
    --openssldir=${PREFIX} \
    shared \
    mingw

echo "make install"
#make clean
#make
make install

cd ${CUR_DIR}
