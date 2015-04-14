#注意：运行前需要设置 msvc 工具链
#   msvc 工具链的环境变量可用下面方法设置：  
#   先从菜单栏中起动vs2013编译命令行工具：  
#   C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts  
#   VS2013 x86 本机工具命令提示  
#   在命令行下，启动msys。 
# 启动 msys 环境：
#        c:\MinGW\msys\1.0\msys.bat
# 或者启动 msys2 环境:
#        c:\msys32\ming32_shell.bat
#   注意，msys中不要装link工具，否则会导致出错。如果有link工具，暂时把它命名成其它名称。  
#   然后再进入脚本目录：`cd ${RabbitImRoot}/ThirdLibary/build_script`。再运行你想运行的编译脚本。例如： `./build.sh` 

#注意：修改后的本文件不要上传代码库中
#需要设置下面变量：
#QT_ROOT=/home/k/Qt5.4.1/5.4/gcc_64 #QT 安装根目录
RABBITIM_MAKE_JOB_PARA="-j2"  #make 同时工作进程参数,建议设置为你机器CUP个数
JOM=make #设置 QT make 工具 JOM

#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

if [ -n "${RabbitImRoot}" ]; then
    RABBITIM_BUILD_PREFIX=${RabbitImRoot}/ThirdLibary/windows_msvc
else
    RABBITIM_BUILD_PREFIX=`pwd`/../windows_msvc    #修改这里为安装前缀 
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

export PATH=${RABBITIM_BUILD_PREFIX}/bin:${QT_BIN}:$PATH
export PKG_CONFIG=/usr/bin/pkg-config
export PKG_CONFIG_PATH=${RABBITIM_BUILD_PREFIX}/lib/pkgconfig
