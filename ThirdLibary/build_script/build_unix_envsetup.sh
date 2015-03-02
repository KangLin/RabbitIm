#注意：修改后的本文件不要上传代码库中
#需要设置下面变量：
QT_ROOT=/home/l/Qt5.3.1/5.3/gcc_64 #QT 安装根目录

JOM=make #设置 QT make 工具 JOM
QT_BIN=${QT_ROOT}/bin       #设置用于 unix 平台编译的qt bin目录
QMAKE=${QT_BIN}/qmake   #设置用于 unix 平台编译的 QMAKE

if [ -n "${RabbitImRoot}" ]; then
    PREFIX=${RabbitImRoot}/ThirdLibary/unix
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

#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

if [ -n "${RabbitImRoot}" ]; then
    RABBITIM_BUILD_PREFIX=${RabbitImRoot}/ThirdLibary/unix
else
    RABBITIM_BUILD_PREFIX=`pwd`/../unix    #修改这里为安装前缀 
fi

export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:${RABBITIM_BUILD_PREFIX}/lib/pkgconfig
