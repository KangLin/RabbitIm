#需要设置下面变量：
#QT_ROOT=/c/Qt/Qt5.3.1/5.3/mingw482_32 #QT 安装根目录
#JOM=/c/Qt/Qt5.3.1/Tools/QtCreator/bin/jom.exe  #设置 QT make 工具 JOM

QT_BIN=${QT_ROOT}/bin       #设置用于 mingw 平台编译的qt bin目录
QMAKE=${QT_BIN}/qmake.exe   #设置用于 mingw 平台编译的 QMAKE
if [ -n "${RabbitImRoot}" ]; then
    PREFIX=${RabbitImRoot}/ThirdLibary/windows_mingw
else
    PREFIX=`pwd`/../windows_mingw    #修改这里为安装前缀 
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
