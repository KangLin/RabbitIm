编译 windows 平台
================

目录
----

1. [windows原生编译](#user-content-windows原生编译)

2. [windows下用mingw交叉编译](#user-content-windows下用mingw交叉编译)

3. [ubuntu下用mingw交叉编译](#user-content-ubuntu下用mingw交叉编译)

windows原生编译
-----------------

### 1. 工具

#### 1.1. 编译工具: msvc
主页：http://msdn.microsoft.com/zh-cn/vstudio  
当前使用版本：vs 2013 update 4

#### 1.2. windows sdk(The Microsoft® Windows® Software Development Kit (SDK) for Windows 8.1):
https://msdn.microsoft.com/zh-cn/windows/desktop/bg162891

#### 1.3. Windows Driver Kit:
http://www.microsoft.com/en-us/download/confirmation.aspx?id=42273

#### 1.4. 汇编工具（yasm、nasm）
[yasm](http://yasm.tortall.net/)  
[nasm](http://www.nasm.us/)  
下载并安装，并且设置路径到环境变量PATH

#### 1.5. perl 工具
http://www.perl.org/

### 2. 环境  
#### 2.1. 操作系统:windows 7 旗舰版 版本：6.1 (内部版本 7601 Service Pack 1)

#### 2.2. bash 环境: msys（或者cygwin）
cygwin主页：http://www.cygwin.org/  
当前使用的是 mingw ，因为 QT for android in windows 预编译包中已包含了 mingw 。只需要下载 msys 。

#### 2.3. 使用:
先从菜单栏中起动vs2013编译命令行工具：  
`C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts VS2013 x86`
本机工具命令提示。在命令行下，启动msys。 
`c:\MinGW\msys\1.0\bin\sh.exe --login -i`  
注意，msys中不要装link工具，否则会导致出错。如果有link工具，暂时把它命名成其它名称。
    
### 3. 编译第三方依赖库(脚本中包括qt)

        cd ${RabbitImRoot}/ThirdLibary/build_script
        ./build.sh unix_mingw [source_code_directory]

### 4. 编译本项目:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh unix_mingw [source_code_directory] [qmake]


windows下用mingw交叉编译
-------------------------
### 1. 环境
#### 1.1. 操作系统:windows 7 旗舰版 版本：6.1 (内部版本 7601 Service Pack 1)
#### 1.2. bash 环境: msys（msys2 或者 cygwin）
* msys: http://www.mingw.org/wiki/MSYS
* msys2: http://sourceforge.net/projects/msys2/
代码位置: https://github.com/Alexpux/MSYS2-packages
* cygwin主页: http://www.cygwin.org/

### 2. 工具
#### 2.1. 交叉编译工具:mingw
* mingw主页： http://www.mingw.org/ 。这个主页好象不是最新的，
* mingw-w64: http://sourceforge.net/projects/mingw-w64/
* mingwbuilds: http://sourceforge.net/projects/mingwbuilds/
因为 QT for android in windows 预编译包中已包含了 mingw 。  
当前使用版本：
gcc version 4.8.2 (i686-posix-dwarf-rev3, Built by MinGW-W64 project)

#### 2.2. 安装auto工具：
主页:
* automake: http://www.gnu.org/software/automake/
* autoconf: http://www.gnu.org/software/autoconf/
* libtool: http://www.gnu.org/software/libtool/
* m4: http://www.gnu.org/software/m4
* pkg-config: http://www.freedesktop.org/wiki/Software/pkg-config/

一般 msys/cygwin 中会包含

#### 2.3. 版本管理工具:
* subversion: http://subversion.apache.org/
* git: http://www.git-scm.com/

#### 2.4. 脚本工具
* python:2.7.6  version 2.7 or later
主页: https://www.python.org/  http://www.activestate.com/activepython/
* perl：5.18.2  version 5.12 or later 
主页： http://www.perl.org/  http://www.activestate.com/activeperl/
* bash:msys或cygwin
* ruby:qtwebkit编译需要 version 1.9.3 or later  
主页：http://www.ruby-lang.org/  http://rubyinstaller.org/  
https://github.com/ruby/ruby

#### 2.5. 汇编工具（yasm、nasm）
[yasm](http://yasm.tortall.net/)  
[nasm](http://www.nasm.us/)  
下载并安装，并且设置路径到环境变量PATH

#### 2.6. cmake 工具
主页：www.cmake.org

        cd /tools
        wget http://www.cmake.org/files/v3.1/cmake-3.1.0-rc1-win32-x86.zip
        unxip cmake-3.1.0-rc1-win32-x86.zip
        mv cmake-3.1.0-rc1-win32-x86 cmake
        export PATH=/tools/cmake/bin:$PATH

#### 2.7. 语法分析工具：bison, flex and gperf（用于Qt编译）

        sudo apt-get install bison flex gperf

#### 2.8. 安装工具
主页：http://nsis.sourceforge.net/Main_Page

### 3. 编译第三方依赖库(脚本中包括qt)

        cd ${RabbitImRoot}/ThirdLibary/build_script
        ./build.sh unix_mingw [source_code_directory]

### 4. 编译本项目:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh unix_mingw [source_code_directory] [qmake]

ubuntu 下用 mingw 交叉编译
---------------------------
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

        cd ${RabbitImRoot}/ThirdLibary/build_script
        ./build.sh unix_mingw [source_code_directory]

### 4. 编译本项目:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh unix_mingw [source_code_directory] [qmake]
