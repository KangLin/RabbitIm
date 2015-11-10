#注意：修改后的本文件不要上传代码库中
#需要设置下面变量：
#QT_ROOT=/home/k/Qt5.3.0/5.3/gcc_64 #QT 安装根目录,默认为:${RabbitImRoot}/ThirdLibary/unix/qt
JOM=make #设置 QT make 工具 JOM
MAKE=make
RABBITIM_MAKE_JOB_PARA="-j2"  #make 同时工作进程参数
RABBITIM_CLEAN=TRUE #编译前清理
#RABBITIM_BUILD_STATIC="static" #设置编译静态库，注释掉，则为编译动态库
#RABBITIM_USE_REPOSITORIES="FALSE" #下载指定的压缩包。省略，则下载开发库。

#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

if [ -n "${RabbitImRoot}" ]; then
    RABBITIM_BUILD_PREFIX=${RabbitImRoot}/ThirdLibary/unix
else
    RABBITIM_BUILD_PREFIX=`pwd`/../unix    #修改这里为安装前缀 
fi
if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
    RABBITIM_BUILD_PREFIX=${RABBITIM_BUILD_PREFIX}_static
fi
if [ ! -d ${RABBITIM_BUILD_PREFIX} ]; then
    mkdir -p ${RABBITIM_BUILD_PREFIX}
fi

if [ -z "$RABBITIM_USE_REPOSITORIES" ]; then
    RABBITIM_USE_REPOSITORIES="TRUE" #下载开发库。省略，则下载指定的压缩包
fi

if [ -z "$QT_ROOT" ]; then
    QT_ROOT=${RABBITIM_BUILD_PREFIX}/qt
fi
QT_BIN=${QT_ROOT}/bin       #设置用于 android 平台编译的 qt bin 目录
QMAKE=${QT_BIN}/qmake       #设置用于 unix 平台编译的 QMAKE。
                            #这里设置的是自动编译时的配置，你需要修改为你本地qt编译环境的配置.
echo "QT_BIN:$QT_BIN"

export PKG_CONFIG=pkg-config 
if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
    export PKG_CONFIG="pkg-config --static"
fi
export PKG_CONFIG_PATH=${RABBITIM_BUILD_PREFIX}/lib/pkgconfig:${PKG_CONFIG_PATH}
export PATH=${QT_BIN}:$PATH

