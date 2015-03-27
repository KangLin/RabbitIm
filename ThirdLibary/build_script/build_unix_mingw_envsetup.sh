#注意：修改后的本文件不要上传代码库中
#需要设置下面变量：
#QT_ROOT=/usr/local/Qt-5.5.0 #QT 安装根目录
JOM=make #设置 QT make 工具 JOM

#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

if [ -n "${RabbitImRoot}" ]; then
    RABBITIM_BUILD_PREFIX=${RabbitImRoot}/ThirdLibary/unix_mingw
else
    RABBITIM_BUILD_PREFIX=`pwd`/../unix_mingw    #修改这里为安装前缀 
fi

if [ ! -d ${RABBITIM_BUILD_PREFIX} ]; then
    mkdir -p ${RABBITIM_BUILD_PREFIX}
fi

if [ -z "$QT_ROOT" ]; then
	QT_ROOT=${RABBITIM_BUILD_PREFIX}/qt
fi
QT_BIN=${QT_ROOT}/bin       #设置用于 unix 平台编译的qt bin目录
QMAKE=${QT_BIN}/qmake   #设置用于 unix 平台编译的 QMAKE

export PATH=${QT_BIN}:$PATH
export PKG_CONFIG=/usr/bin/pkg-config
export PKG_CONFIG_PATH=${RABBITIM_BUILD_PREFIX}/lib/pkgconfig

if [ -z "${RABBITIM_BUILD_CROSS_HOST}" ]; then
	RABBITIM_BUILD_CROSS_HOST=i686-w64-mingw32
fi
RABBITIM_BUILD_CROSS_PREFIX=${RABBITIM_BUILD_CROSS_HOST}- 
