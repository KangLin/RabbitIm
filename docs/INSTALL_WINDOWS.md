## 目录
1. [windows原生编译](#user-content-windows原生编译)

2. [windows下用mingw交叉编译](#user-content-windows下用mingw交叉编译)

3. [ubuntu下用mingw交叉编译](#user-content-ubuntu下用mingw交叉编译)

## windows原生编译

### 1. windows平台工具
* msvc：
主页：http://msdn.microsoft.com/zh-cn/vstudio
当前使用版本：vs 2013 update 4

* windows sdk(The Microsoft® Windows® Software Development Kit (SDK) for Windows 8.1):
https://msdn.microsoft.com/zh-cn/windows/desktop/bg162891

* Windows Driver Kit:
http://www.microsoft.com/en-us/download/confirmation.aspx?id=42273

* msys（或者cygwin）：
mingw主页：http://www.mingw.org/ 。这个主页好象不是最新的，请用：http://sourceforge.net/projects/mingw-w64/  
cygwin主页：http://www.cygwin.org/  
当前使用的是 mingw ，因为 QT for android in windows 预编译包中已包含了 mingw 。只需要下载 msys 。

* 汇编工具（yasm、nasm）
[yasm](http://yasm.tortall.net/)
[nasm](http://www.nasm.us/)  
下载并安装，并且设置路径到环境变量PATH

### 2. 启动环境  
先从菜单栏中起动vs2013编译命令行工具：  
`C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts VS2013 x86`
本机工具命令提示。在命令行下，启动msys。 
`c:\MinGW\msys\1.0\bin\sh.exe --login -i`  
注意，msys中不要装link工具，否则会导致出错。如果有link工具，暂时把它命名成其它名称。
    
* 编译第三方依赖库
+ build_windows_msvc_envsetup.sh:设置编译时需要的变量  
+ build.sh windows_msvc [source_code_directory] : 编译第三方库脚本
    
        cd ${RabbitImRoot}/ThirdLibary/build_script
        ./build.sh windows_msvc [source_code_directory]

## windows下用mingw交叉编译

## ubuntu下用mingw交叉编译

### 1. 安装交叉编译工具mingw

        sudo apt-get install mingw-w64

### 2. 其它工具详见：[INSTALL_UBUNTU.md](INSTALL_UBUNTU.md#user-content-工具)

### 3. qt 编译

####    3.1. 工具：

        sudo apt-get install bison flex gperf ruby icu-devtools

####    3.2. 依赖库：
* openssl
* libicu

####    3.3. qt编译：

        cd ${RabbitImRoot}/ThirdLibary/build_script
        ./build_qt.sh unix_mingw [source_code_directory]
    
### 4. 编译第三方依赖库(脚本中包括qt)

        cd ${RabbitImRoot}/ThirdLibary/build_script
        ./build.sh unix_mingw [source_code_directory]
