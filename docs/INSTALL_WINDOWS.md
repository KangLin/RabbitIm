编译 windows 平台
================

目录
----

1. [windows原生编译](#user-content-windows原生编译)
2. [windows下用mingw交叉编译](#user-content-windows下用mingw交叉编译)
3. [ubuntu下用mingw交叉编译](#user-content-ubuntu下用mingw交叉编译)

windows原生编译
--------------

### 1. 环境
#### 1.1. 操作系统:windows 7 旗舰版 版本：6.1 (内部版本 7601 Service Pack 1)

### 2. 工具
#### 2.1. bash 环境: msys（msys2） 或者 cygwin
* msys: http://www.mingw.org/wiki/MSYS
* msys2: http://sourceforge.net/projects/msys2/
代码位置: https://github.com/Alexpux/MSYS2-packages
* cygwin主页: http://www.cygwin.org/

当前文档以msys2为例：
[安装步骤](http://sourceforge.net/p/msys2/wiki/MSYS2%20installation/)
+ 从官网下载[msys2](http://sourceforge.net/projects/msys2/files/Base/i686/)
+ 安装，启动msys2
+ 同步：

    pacman -Sy

+ 更新系统：

    pacman -Su

+ 下载工具：

    pacman -S wget subversion git autoconf automake m4 libtool pkg-config make bison flex gperf unzip

+ 下载本地编译器gcc（版本：4.9.2）：

    pacman -S mingw-w64-i686-gcc

+ 可能会出现的问题：
[出现资源不足](http://sourceforge.net/p/msys2/tickets/74/)：  
39 [main] make 7628 child_info_fork::abort: C:\Users\AndreaZ\Documents\msys2_32\usr\bin\msys-unistring-2.dll: Loaded to different address: parent(0x440000) != child(0x630000)  
make: fork: Resource temporarily unavailable  
解决方法： 
. 关闭所有msys2进程  
. 运行 autorebase.bat  
. 重启 MSYS2  

最好在 pacman -Su 后就做 autorebase.bat

#### 2.1. 编译工具: msvc
主页：http://msdn.microsoft.com/zh-cn/vstudio  
当前使用版本：vs 2013 update 4

#### 2.3. windows sdk(The Microsoft® Windows® Software Development Kit (SDK) for Windows 8.1):
https://msdn.microsoft.com/zh-cn/windows/desktop/bg162891

#### 2.4. Windows Driver Kit:
http://www.microsoft.com/en-us/download/confirmation.aspx?id=42273

#### 2.5. 下载工具：

    pacman -S wget

#### 2.6. 安装auto工具：
主页:
* automake: http://www.gnu.org/software/automake/
* autoconf: http://www.gnu.org/software/autoconf/
* libtool: http://www.gnu.org/software/libtool/
* m4: http://www.gnu.org/software/m4
* pkg-config: http://www.freedesktop.org/wiki/Software/pkg-config/
* make:根据bash系统的不同，有msys make（msys bash）、mingw make、gnu make（cygwin bash）

或者：

    pacman -S autoconf automake m4 libtool pkg-config make

#### 2.7. 版本管理工具:
* subversion: http://subversion.apache.org/
* git: http://www.git-scm.com/

或者：

    pacman -S subversion git

#### 2.8. 脚本工具
* python:2.7.6  版本 2.7 或以后
主页: https://www.python.org/  http://www.activestate.com/activepython/
* perl：5.18.2  版本 5.12 或以后
主页： http://www.perl.org/  http://www.activestate.com/activeperl/
* bash:msys或cygwin
* ruby:qtwebkit编译需要 版本 1.9.3 或以后
主页：http://www.ruby-lang.org/  http://rubyinstaller.org/  
https://github.com/ruby/ruby

或者：

    pacman -S python perl ruby

#### 2.9. 汇编工具（yasm、nasm）
[yasm](http://yasm.tortall.net/)  
[nasm](http://www.nasm.us/)  
下载并安装，并且设置路径到环境变量PATH

或者：

    pacman yasm nasm

#### 2.10. cmake 工具
主页：www.cmake.org

    cd /tools
    wget http://www.cmake.org/files/v3.1/cmake-3.1.0-rc1-win32-x86.zip
    unzip cmake-3.1.0-rc1-win32-x86.zip
    mv cmake-3.1.0-rc1-win32-x86 cmake
    export PATH=/tools/cmake/bin:$PATH

或者：

    pacman -S mingw-w64-i686-cmake

#### 2.11. 语法分析工具：bison, flex and gperf（用于Qt编译）

    pacman -S bison flex gperf

#### 2.12. 解压缩工具：unzip

    pacman -S unzip

#### 2.13. 安装工具
主页：http://nsis.sourceforge.net/Main_Page

### 3. 使用:
先从菜单栏中起动vs2013编译命令行工具：  
`C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts VS2013 x86`
本机工具命令提示。在命令行下，启动msys:`c:\MinGW\msys\1.0\msys.bat`  
如果用msys2：`c:\msys32\mingw32_shell.bat`  
注意，msys中不要装link工具，否则会导致出错。如果有link工具，暂时把它命名成其它名称。
    
### 4. 编译第三方依赖库(脚本中包括qt)

1. build_envsetup_windows_msvc.sh:设置编译时需要的变量  
2. build.sh windows_msvc [source_code_directory] : 编译第三方库脚本
3. build_XXX.sh windows_msvc [source_code_directory] : 编译指定的 XXX 第三方库脚本

编译所有依赖的第三方库：

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build.sh windows_msvc [source_code_directory]

或者只编译其中一个库，例如：libvpx 

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_libvpx.sh windows_msvc [source_code_directory]

环境变量说明：

1. QT_ROOT=                          #qt的安装位置,默认为:${RabbitImRoot}/ThirdLibary/windows_msvc/qt
2. RABBITIM_MAKE_JOB_PARA="-j2"      #make 同时工作进程参数,建议设置为你机器CUP个数
3. RABBITIM_BUILD_STATIC="static"    #设置编译静态库，注释掉，则为编译动态库
4. JOM=make                          #设置 QT make 工具 JOM
5. RABBITIM_USE_REPOSITORIES="FALSE" #下载指定的压缩包。省略，则下载开发库。
6. RABBITIM_CLEAN=TRUE               #编译前清理

### 5. 编译本项目:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh windows_msvc [source_code_directory] [qmake]

windows下用mingw交叉编译
----------------------

### 1. 环境
#### 1.1. 操作系统:windows 7 旗舰版 版本：6.1 (内部版本 7601 Service Pack 1)
#### 1.2. bash 环境: msys（msys2） 或者 cygwin
* msys: http://www.mingw.org/wiki/MSYS
* msys2: http://sourceforge.net/projects/msys2/
代码位置: https://github.com/Alexpux/MSYS2-packages
* cygwin主页: http://www.cygwin.org/

当前文档以msys2为例：
[安装步骤](http://sourceforge.net/p/msys2/wiki/MSYS2%20installation/)
+ 从官网下载[msys2](http://sourceforge.net/projects/msys2/files/Base/i686/)
+ 安装，启动msys2
+ 同步：

    pacman -Sy

+ 更新系统：

    pacman -Su

+ 下载工具：

    pacman -S wget subversion git autoconf automake m4 libtool pkg-config make mingw-w64-i686-make bison flex gperf

+ 下载本地编译器gcc（版本：4.9.2）：

    pacman -S mingw-w64-i686-gcc

+ 可能会出现的问题：
[出现资源不足](http://sourceforge.net/p/msys2/tickets/74/)：
39 [main] make 7628 child_info_fork::abort: C:\Users\AndreaZ\Documents\msys2_32\usr\bin\msys-unistring-2.dll: Loaded to different address: parent(0x440000) != child(0x630000)  
make: fork: Resource temporarily unavailable
解决方法：
. 关闭所有msys2进程
. 运行 autorebase.bat
. 重启 MSYS2:`c:\msys32\mingw32_shell.bat`

最好在 pacman -Su 后就做 autorebase.bat

### 2. 工具
#### 2.1. 交叉编译工具:mingw
* mingw主页： http://www.mingw.org/ 。这个主页好象不是最新的，
* mingw-w64: http://sourceforge.net/projects/mingw-w64/
* mingwbuilds: http://sourceforge.net/projects/mingwbuilds/
因为 QT for android in windows 预编译包中已包含了 mingw 。  
当前使用版本：
gcc version 4.8.2 (i686-posix-dwarf-rev3, Built by MinGW-W64 project)

msys2中有自带的mingw：gcc.exe (Rev5, Built by MSYS2 project) 4.9.2

    pacman -S mingw-w64-i686-gcc

cygwin中安装也自带有的mingw：i686-w64-mingw32-gcc (GCC) 4.8.3

#### 2.2. 下载工具：

    pacman -S wget

#### 2.3. 安装auto工具：
主页:
* automake: http://www.gnu.org/software/automake/
* autoconf: http://www.gnu.org/software/autoconf/
* libtool: http://www.gnu.org/software/libtool/
* m4: http://www.gnu.org/software/m4
* pkg-config: http://www.freedesktop.org/wiki/Software/pkg-config/
* make:根据bash系统的不同，有msys make（msys bash）、mingw32-make、gnu make（cygwin bash）

或者：

    pacman -S autoconf automake m4 libtool pkg-config make mingw-w64-i686-make

#### 2.4. 版本管理工具:
* subversion: http://subversion.apache.org/
* git: http://www.git-scm.com/

或者：

    pacman -S subversion git

#### 2.5. 脚本工具
* python:2.7.6  版本 2.7 或以后
主页: https://www.python.org/  http://www.activestate.com/activepython/
* perl：5.18.2  版本 5.12 或以后
主页： http://www.perl.org/  http://www.activestate.com/activeperl/
* bash:msys或cygwin
* ruby:qtwebkit编译需要 版本 1.9.3 或以后
主页：http://www.ruby-lang.org/  http://rubyinstaller.org/  
https://github.com/ruby/ruby

或者：

    pacman -S python perl ruby

#### 2.6. 汇编工具（yasm、nasm）
[yasm](http://yasm.tortall.net/)  
[nasm](http://www.nasm.us/)  
下载并安装，并且设置路径到环境变量PATH

或者：

    pacman yasm nasm

#### 2.7. cmake 工具
主页：www.cmake.org

    cd /tools
    wget http://www.cmake.org/files/v3.1/cmake-3.1.0-rc1-win32-x86.zip
    unzip cmake-3.1.0-rc1-win32-x86.zip
    mv cmake-3.1.0-rc1-win32-x86 cmake
    export PATH=/tools/cmake/bin:$PATH

或者：

    pacman -S mingw-w64-i686-cmake

#### 2.8. 语法分析工具：bison, flex and gperf（用于Qt编译）

    pacman -S bison flex gperf

#### 2.9. 解压缩工具：unzip

    pacman -S unzip

#### 2.10. 安装工具
主页：http://nsis.sourceforge.net/Main_Page

### 3. 编译
#### 3.1 启动下列环境之一：
* 启动 msys 环境：

    c:\MinGW\msys\1.0\msys.bat

* 启动 msys2 32环境:

    c:\msys32\mingw32_shell.bat #32位编译环境

* 启动 msys2 64环境:

    c:\msys32\mingw64_shell.bat #64位编译环境

#### 3.2 编译第三方依赖库(脚本中包括qt)

编译脚本：

1. build.sh windows_mingw [source_code_directory] : 编译第三方库脚本
2. build_envsetup_windows_mingw.sh:设置编译时需要的变量
3. build_XXX.sh windows_mingw [source_code_directory] : 编译指定的 XXX 第三方库脚本

环境变量说明： 

1. QT_ROOT=                          #qt的安装位置,默认为:${RabbitImRoot}/ThirdLibary/windows_mingw/qt
2. RABBITIM_MAKE_JOB_PARA="-j2"      #make 同时工作进程参数,建议设置为你机器CUP个数
3. RABBITIM_BUILD_STATIC="static"    #设置编译静态库，注释掉，则为编译动态库
4. JOM=make                          #设置 QT make 工具 JOM
5. RABBITIM_USE_REPOSITORIES="FALSE" #下载指定的压缩包。省略，则下载开发库。
6. RABBITIM_CLEAN=TRUE               #编译前清理

编译所有依赖的第三方库：

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build.sh windows_mingw [source_code_directory]

或者只编译其中一个库，例如：libvpx

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_libvpx.sh windows_mingw [source_code_directory]

#### 3.3. 编译本项目:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh windows_mingw [source_code_directory] [qmake]

ubuntu 下用 mingw 交叉编译
------------------------

### 1. [环境](INSTALL_UBUNTU.md#user-content-环境)

### 2. 工具

#### 2.1. 安装交叉编译工具 mingw ,当前使用版本:4.8.2

    sudo apt-get install mingw-w64

#### 2.2. auto工具：

    sudo apt-get install autoconf automake libtool m4
    sudo apt-get install pkg-config

#### 2.3. 版本管理工具：

    sudo apt-get install subversion git

#### 2.4. 脚本工具：
* python:2.7.6
* perl:5.18.2
* bash:
* ruby:qtwebkit编译需要

    sudo apt-get install ruby build-essential perl python 

#### 2.5. 汇编工具：

    sudo apt-get install yasm

#### 2.6. 下载工具

    sudo apt-get install wget

#### 2.7. cmake 工具

    cd /home
    wget http://www.cmake.org/files/v3.1/cmake-3.1.0-Linux-x86_64.tar.gz
    tar xf cmake-3.1.0-Linux-x86_64.tar.gz
    mv cmake-3.1.0-Linux-x86_64 cmake
    export PATH=/home/cmake/bin:$PATH

#### 2.8. 安装工具

    sudo apt-get install nsis

#### 2.9. 语法分析工具：bison, flex and gperf（用于Qt编译）

    sudo apt-get install bison flex gperf

#### 2.10. icu(qtwebkit编译需要)

    sudo apt-get install icu-devtools #脚本

### 3. 编译第三方依赖库(脚本中包括qt)
编译脚本：

1. build.sh windows_mingw [source_code_directory] : 编译第三方库脚本
2. build_envsetup_windows_mingw.sh:设置编译时需要的变量
3. build_XXX.sh windows_mingw [source_code_directory] : 编译指定的 XXX 第三方库脚本

环境变量说明： 

1. QT_ROOT=                          #qt的安装位置,默认为:${RabbitImRoot}/ThirdLibary/windows_mingw/qt
2. RABBITIM_MAKE_JOB_PARA="-j2"      #make 同时工作进程参数,建议设置为你机器CUP个数
3. RABBITIM_BUILD_STATIC="static"    #设置编译静态库，注释掉，则为编译动态库
4. JOM=make                          #设置 QT make 工具 JOM
5. RABBITIM_USE_REPOSITORIES="FALSE" #下载指定的压缩包。省略，则下载开发库。
6. RABBITIM_CLEAN=TRUE               #编译前清理
7. RABBITIM_BUILD_CROSS_HOST=        #编译工具链前缀,用于交叉编译，默认为：i686-w64-mingw32

编译所有依赖的第三方库：

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build.sh windows_mingw [source_code_directory]

或者只编译其中一个库，例如：libvpx

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_libvpx.sh windows_mingw [source_code_directory]

### 4. 编译本项目:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh windows_mingw [source_code_directory] [qmake]
