#注意：修改后的本文件不要上传代码库中
#需要设置下面变量：
QMAKE=/home/k/Qt5.3.0/5.3/gcc_64/bin/qmake #设置用于 unix 平台编译的 QMAKE。
                                             #这里设置的是自动编译时的配置，你需要修改为你本地qt编译环境的配置.
                                             #注意：修改后的本文件不要上传代码库中
JOM=make                 #设置 QT make 工具 JOM

if [ -n "${RabbitImRoot}" ]; then
    PREFIX=${RabbitImRoot}/ThirdLibary/uinx
else
    PREFIX=`pwd`/../unix    #修改这里为安装前缀 
fi

if [ ! -d ${PREFIX} ]; then
    mkdir -p ${PREFIX}
fi

export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
export QMAKE

echo ""
echo "PREFIX:$PREFIX"
echo "QMAKE:$QMAKE"
echo "JOM:$JOM"
echo ""
