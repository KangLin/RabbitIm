#注意：运行前需要设置 msvc 工具链
#   msvc 工具链的环境变量可用下面方法设置：  
#   先从菜单栏中起动vs2013编译命令行工具：  
#   C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts  
#   VS2013 x86 本机工具命令提示  
#   在命令行下，启动msys。 
#   `c:\MinGW\msys\1.0\msys`  
#   注意，msys中不要装link工具，否则会导致出错。如果有link工具，暂时把它命名成其它名称。  
#   然后再进入脚本目录：`cd ${RabbitImRoot}/ThirdLibary/build_script`。再运行你想运行的编译脚本。例如： `./build_windows_mscv.sh` 

if [ -n "${RabbitImRoot}" ]; then
    PREFIX=${RabbitImRoot}/ThirdLibary/windows
else
    PREFIX=`pwd`/../windows    #修改这里为安装前缀 
fi

if [ ! -d ${PREFIX} ]; then
    mkdir -p ${PREFIX}
fi

echo ""
echo "PREFIX:$PREFIX"
echo ""
