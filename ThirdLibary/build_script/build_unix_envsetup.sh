#注意：修改后的本文件不要上传代码库中
#需要设置下面变量：
#QT_ROOT=/home/l/Qt5.3.1/5.3/gcc_64 #QT 安装根目录

JOM=make #设置 QT make 工具 JOM
QT_BIN=${QT_ROOT}/bin       #设置用于 unix 平台编译的qt bin目录
QMAKE=${QT_BIN}/qmake   #设置用于 unix 平台编译的 QMAKE

if [ -n "${RabbitImRoot}" ]; then
    PREFIX=${RabbitImRoot}/ThirdLibary/uinx
else
    PREFIX=`pwd`/../unix    #修改这里为安装前缀 
fi

if [ ! -d ${PREFIX} ]; then
    mkdir -p ${PREFIX}
fi

export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
export PATH=${QT_BIN}:$PATH

echo ""
echo "PREFIX:$PREFIX"
echo "QMAKE:$QMAKE"
echo "JOM:$JOM"
echo ""
