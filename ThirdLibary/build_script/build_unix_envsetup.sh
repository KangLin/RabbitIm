#需要设置下面变量：
#QMAKE=/c/Qt/Qt5.3.0_android/5.3/mingw482_32/bin/qmake.exe  #设置用于 mingw 平台编译的 QMAKE
#JOM=/c/Qt/Qt5.3.0_android/Tools/QtCreator/bin/jom.exe                 #设置 QT make 工具 JOM

if [ -n "${RabbitImRoot}" ]; then
    PREFIX=${RabbitImRoot}/ThirdLibary/uinx
else
    PREFIX=`pwd`/../unix    #修改这里为安装前缀 
fi

if [ ! -d ${PREFIX} ]; then
    mkdir -p ${PREFIX}
fi

export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig

echo ""
echo "PREFIX:$PREFIX"
echo "QMAKE:$QMAKE"
echo "JOM:$JOM"
echo ""
