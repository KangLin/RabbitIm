#参数:
#    $1:源码的位置 

#注意：运行前需要设置 msvc 工具链
#   msvc 工具链的环境变量可用下面方法设置：  
#   先从菜单栏中起动vs2013编译命令行工具：  
#   C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts  
#   VS2013 x86 本机工具命令提示  
#  
#   然后再进入脚本目录：`cd ${RabbitImRoot}/ThirdLibary/build_script`。再运行你想运行的编译脚本。例如： `./build_windows_mscv.sh` 
#
#需要 perl

#在windows命令行下执行下面步骤

#运行本脚本前,先运行 build_windows_msvc_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   PREFIX=`pwd`/../windows  #修改这里为安装前缀
if [ -z "${PREFIX}" ]; then
    echo "build_windows_msvc_envsetup.sh"
    source build_windows_msvc_envsetup.sh
fi

if [ -n "$1" ]; then
    SOURCE_CODE=$1
else
    SOURCE_CODE=${PREFIX}/../src/openssl
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone https://github.com/openssl/openssl"
    git clone https://github.com/openssl/openssl ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo ""

nmake clean
echo "configure openssl ..."
perl Configure \
    --prefix=${PREFIX} \
    --openssldir=${PREFIX} \
    VC-WIN32

ms/do_nasm.bat

echo "make install"
#静态库
#nmake -f ms/nt.mak install﻿﻿
#动态库  
nmake -f ms/ntdll.mak #install﻿﻿

cd ${CUR_DIR}
