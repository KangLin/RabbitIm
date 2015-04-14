#注意：修改后的本文件不要上传代码库中
#需要设置下面变量：
#QT_ROOT=/home/k/Qt5.4.1/5.4/gcc_64 #QT 安装根目录
#RABBITIM_MAKE_JOB_PARA="-j2"  #make 同时工作进程参数,建议设置为你机器CUP个数
JOM=nmake #设置 QT make 工具 JOM

#RABBITIM_BUILD_CROSS_HOST=i686-w64-mingw32  #编译工具链前缀

#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

if [ -n "${RabbitImRoot}" ]; then
    RABBITIM_BUILD_PREFIX=${RabbitImRoot}/ThirdLibary/windows_mingw
else
    RABBITIM_BUILD_PREFIX=`pwd`/../windows_mingw    #修改这里为安装前缀 
fi
if [ ! -d ${RABBITIM_BUILD_PREFIX} ]; then
    mkdir -p ${RABBITIM_BUILD_PREFIX}
fi

#自动判断主机类型，目前只做了linux、windows判断
TARGET_OS=`uname -s`
case $TARGET_OS in
    MINGW* | CYGWIN* | MSYS*)
        ;;
    Linux* | Unix*)
        if [ -z $RABBITIM_MAKE_JOB_PARA ]; then
            RABBITIM_MAKE_JOB_PARA="-j2"  #make 同时工作进程参数,建议设置为你机器CUP个数
        fi
        ;;
    *)
    echo "Please set RABBITIM_BUILD_HOST. see build_envsetup_windows_mingw.sh"
    return 2
    ;;
esac

if [ -z "$QT_ROOT" ]; then
        QT_ROOT=${RABBITIM_BUILD_PREFIX}/qt
fi
QT_BIN=${QT_ROOT}/bin       #设置用于 android 平台编译的 qt bin 目录
QMAKE=${QT_BIN}/qmake       #设置用于 unix 平台编译的 QMAKE。
                            #这里设置的是自动编译时的配置，你需要修改为你本地qt编译环境的配置.

export PATH=${RABBITIM_BUILD_PREFIX}/bin:${QT_BIN}:$PATH
export PKG_CONFIG=/usr/bin/pkg-config
export PKG_CONFIG_PATH=${RABBITIM_BUILD_PREFIX}/lib/pkgconfig:$PKG_CONFIG_PATH

if [ -z "${RABBITIM_BUILD_CROSS_HOST}" ]; then
	RABBITIM_BUILD_CROSS_HOST=i686-w64-mingw32 #编译工具链前缀
fi
RABBITIM_BUILD_CROSS_PREFIX=${RABBITIM_BUILD_CROSS_HOST}- 
