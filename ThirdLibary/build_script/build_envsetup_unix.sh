#注意：修改后的本文件不要上传代码库中
#需要设置下面变量：
#QT_ROOT=/home/k/Qt5.4.1/5.4/gcc_64 #QT 安装根目录
JOM=make #设置 QT make 工具 JOM
RABBITIM_MAKE_JOB_PARA="-j2"  #make 同时工作进程参数

#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

if [ -n "${RabbitImRoot}" ]; then
    RABBITIM_BUILD_PREFIX=${RabbitImRoot}/ThirdLibary/unix
else
    RABBITIM_BUILD_PREFIX=`pwd`/../unix    #修改这里为安装前缀 
fi
if [ ! -d ${RABBITIM_BUILD_PREFIX} ]; then
    mkdir -p ${RABBITIM_BUILD_PREFIX}
fi

if [ -z "$QT_ROOT" ]; then
        QT_ROOT=${RABBITIM_BUILD_PREFIX}/qt
fi
QT_BIN=${QT_ROOT}/bin       #设置用于 android 平台编译的 qt bin 目录
QMAKE=${QT_BIN}/qmake       #设置用于 unix 平台编译的 QMAKE。
                            #这里设置的是自动编译时的配置，你需要修改为你本地qt编译环境的配置.
                            
export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:${RABBITIM_BUILD_PREFIX}/lib/pkgconfig
export PATH=${QT_BIN}:$PATH

