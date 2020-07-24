编译 ANDROID 平台
================

## 目录

1. [编译或下载第三方库](#编译或下载第三方库)
2. [ubuntu下交叉编译](#ubuntu下交叉编译)
3. [windows下交叉编译](#windows下交叉编译)

## 编译或下载第三方库

- [必须]RabbitCommon

    git clone https://github.com/KangLin/RabbitCommon.git

- 编译或下载第三方库
  + [编译第三方库](https://github.com/KangLin/RabbitThirdLibrary/blob/master/INSTALL_ANDROID.md)
  + [下载第三方库](https://github.com/KangLin/RabbitThirdLibrary/releases/latest)

## ubuntu下交叉编译

### 工具

#### java
* 下载：[jdk](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* 设置环境变量：

    export JAVA_HOME=/home/jdk1.7.0_51
    export PATH=$JAVA_HOME/bin:$PATH

#### android ndk

* 工具链版本：4.9
* 下载:

    cd /home
    wget https://dl.google.com/android/repository/android-ndk-r17-linux-x86_64.zip
    unzip android-ndk-r17-linux-x86_64.zip
    mv android-ndk-r17 android-ndk


* 设置环境变量

    export ANDROID_NDK=/home/android-ndk
    export ANDROID_NDK_ROOT=$ANDROID_NDK


#### android sdk

* 平台版本：24
* 下载：

    cd /home
    wget https://dl.google.com/android/android-sdk_r24.4.1-linux.tgz
    tar xvf android-sdk_r24.4.1-linux.tgz
    mv android-sdk-linux android-sdk

* 设置环境变量

    export ANDROID_SDK=/home/android-sdk/sdk
    export ANDROID_SDK_ROOT=$ANDROID_SDK
    export PATH=$PATH:$ANDROID_SDK/platform-tools

注意：
1. qt与第三方依赖库所使用的工具链版本需要一样。(当前Qt5.5.1用的android ndk 9）。
构建->构建环境变量:设置 ANDROID_NDK_PLATFORM 到 android-15

#### 安装32位库(可选): ubuntu 64位下 android sdk 需要

    sudo apt-get install zlib1g:i386 libstdc++6:i386 libc6:i386
    
#### ant工具(Qt5.9.0以上使用 gradle)

    sudo apt-get install ant

#### 下载工具

    sudo apt-get install wget

#### auto工具：

    sudo apt-get install autoconf automake libtool m4
    sudo apt-get install pkg-config

#### 版本管理工具：

    sudo apt-get install subversion git

[git设置](http://blog.csdn.net/kl222/article/details/32903495)

#### 脚本工具：

* Perl version 5.12 or later   [http://www.activestate.com/activeperl/]
* Python version 2.7 or later  [http://www.activestate.com/activepython/]
* Ruby version 1.9.3 or later  [http://rubyinstaller.org/]  
    qtwebkit编译需要
* bash:

    sudo apt-get install ruby  build-essential perl python 

#### cmake 工具
版本：大于等3.1.0

    cd /home
    wget http://www.cmake.org/files/v3.1/cmake-3.1.0-Linux-x86_64.tar.gz
    tar xf cmake-3.1.0-Linux-x86_64.tar.gz
    mv cmake-3.1.0-Linux-x86_64 cmake
    export PATH=/home/cmake/bin:$PATH


### 环境:详见:[INSATLL_UBUNTU.md](INSTALL_UBUNTU.md#user-content-环境)

### 编译本项目:

- 命令行编译：

    $QT_ROOT/bin/qmake THIRD_LIBRARY_PATH= [RabbitCommon_DIR=...]
    make

- 用 QtCreator
  + 打开 QtCreator,并打开项目 RabbitIm.pro
  + 设置参数 THIRD_LIBRARY_PATH=
  + 编译

## windows下交叉编译

### 环境
#### 操作系统:windows 7 旗舰版 版本：6.1 (内部版本 7601 Service Pack 1)
#### bash 环境: msys（msys2） 或者 cygwin
* msys: http://www.mingw.org/wiki/MSYS
* msys2: http://sourceforge.net/projects/msys2/
代码位置: https://github.com/Alexpux/MSYS2-packages
* cygwin主页: http://www.cygwin.org/

#### 当前文档以msys2为例：

##### 安装

[安装步骤](http://sourceforge.net/p/msys2/wiki/MSYS2%20installation/)

- 从官网下载[msys2](http://sourceforge.net/projects/msys2/files/Base/i686/)
- 设置国内镜像：

打开 ${MSYS2_ROOT}/etc/pacman.d/mirrorlist.msys

    Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/msys/$arch
    Server = http://mirror.bit.edu.cn/msys2/REPOS/MSYS2/$arch
    Server = http://mirrors.ustc.edu.cn/msys2/REPOS/MSYS2/$arch

打开 ${MSYS2_ROOT}/etc/pacman.d/mirrorlist.mingw32

    Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/i686
    Server = http://mirror.bit.edu.cn/msys2/REPOS/MINGW/i686/
    Server = http://mirrors.ustc.edu.cn/msys2/REPOS/MINGW/i686/

打开 ${MSYS2_ROOT}/etc/pacman.d/mirrorlist.mingw64
    
    Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/x86_64
    Server = http://mirror.bit.edu.cn/msys2/REPOS/MINGW/x86_64
    Server = http://mirrors.ustc.edu.cn/msys2/REPOS/MINGW/x86_64/

##### 启动msys2

+ 启动命令

    d:\msys32>msys2_shell.cmd --help
    Usage:
        msys2_shell.cmd [options] [bash parameters]

    Options:
        -mingw32 | -mingw64 | -msys[2]   Set shell type
        -defterm | -mintty | -conemu | -consolez
                                         Set terminal type
        -here                            Use current directory as working
                                         directory
        -where DIRECTORY                 Use specified DIRECTORY as working
                                         directory
        -[use-]full-path                 Use full currnent PATH variable
                                         instead of triming to minimal
        -help | --help | -? | /?         Display this help and exit

    Any parameter that cannot be treated as valid option and all
    following parameters are passed as bash command parameters.

+ 启动

    msys2_shell.cmd -msys2 -mintty

+ 同步：

    pacman -Sy

+ 更新系统：

    pacman -Su

+ 下载工具：

    pacman -S wget subversion git mingw-w64-i686-pkg-config mingw-w64-i686-make bison flex gperf unzip

+ 可能会出现的问题：
[出现资源不足](http://sourceforge.net/p/msys2/tickets/74/)：
39 [main] make 7628 child_info_fork::abort: C:\Users\AndreaZ\Documents\msys2_32\usr\bin\msys-unistring-2.dll: Loaded to different address: parent(0x440000) != child(0x630000)  
make: fork: Resource temporarily unavailable
解决方法：
. 关闭所有msys2进程
. 运行 autorebase.bat
. 重启 MSYS2

最好在 pacman -Su 后就做 autorebase.bat

### 工具
#### java

* 下载：[jdk](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* 设置环境变量：

    vim ~/.bashrc
    export JAVA_HOME="/C/Program Files/Java/jdk1.7.0_51" 
    export PATH=$JAVA_HOME/bin:$PATH

注意：在msys2下需要注意路径符号,用unix型路径："/"

#### android ndk

* 工具链版本：4.9
* 下载:

    cd /home
    wget https://dl.google.com/android/repository/android-ndk-r17b-windows-x86_64.zip
    unzip android-ndk-r17b-windows-x86_64.zip
    mv android-ndk-r17b android-ndk

* 设置环境变量

    vim ~/.bashrc
    export ANDROID_NDK=/D/software/android-ndk
    export ANDROID_NDK_ROOT=$ANDROID_NDK
    export PATH=$ANDROID_SDK/platform-tools:$PATH

注意：
1. 在msys2下需要注意路径符号,用unix型路径："/"
2. qt与第三方依赖库所使用的工具链版本需要一样。(当前Qt5.5.1用的android ndk 9）。
构建->构建环境变量:设置 ANDROID_NDK_PLATFORM 到 android-18

#### android sdk

* 平台版本：24.4.1
* 下载：

    wget https://dl.google.com/android/android-sdk_r24.4.1-windows.zip
    unzip android-sdk_r24.4.1-windows.zip
    mv android-sdk-windows android-sdk
    rm android-sdk_r24.4.1-windows.zip

* 设置环境变量

    vim ~/.bashrc
    export ANDROID_SDK=/D/software/android-sdk
    export ANDROID_SDK_ROOT=$ANDROID_SDK

注意：
1. 在msys2下需要注意路径符号,用unix型路径："/"。
2. qt与第三方依赖库所使用的工具链版本需要一样。(当前Qt5.5.1用的android ndk 9）。
构建->构建环境变量:设置 ANDROID_NDK_PLATFORM 到 android-18

#### 下载工具

主页： http://www.gnu.org/software/wget/  
一般 msys/cygwin 中会包含

#### ant工具

主页： http://ant.apache.org/

    cd /tools
    wget http://apache.dataguru.cn//ant/binaries/apache-ant-1.9.4-bin.zip
    vim ~/.bashrc
    #并把路径加到环境变量PATH中
    export PATH=/tools/ant/bin:$PATH

#### auto工具：

主页:
* automake: http://www.gnu.org/software/automake/
* autoconf: http://www.gnu.org/software/autoconf/
* libtool: http://www.gnu.org/software/libtool/
* m4: http://www.gnu.org/software/m4
* pkg-config: http://www.freedesktop.org/wiki/Software/pkg-config/
* make:当前用的是msys make

一般 msys/cygwin 中会包含

#### 版本管理工具：

* subversion: http://subversion.apache.org/
* git: http://www.git-scm.com/

需要把路径加到环境变量 PATH 中

#### 脚本工具：

* python:2.7.6  version 2.7 or later
主页: https://www.python.org/  http://www.activestate.com/activepython/
* perl：5.18.2  version 5.12 or later 
主页： http://www.perl.org/  http://www.activestate.com/activeperl/
* bash:msys或cygwin
* ruby:qtwebkit编译需要 version 1.9.3 or later  
主页：http://www.ruby-lang.org/  http://rubyinstaller.org/  
https://github.com/ruby/ruby

#### cmake 工具

    cd /tools
    wget http://www.cmake.org/files/v3.1/cmake-3.1.0-rc1-win32-x86.zip
    unxip cmake-3.1.0-rc1-win32-x86.zip
    mv cmake-3.1.0-rc1-win32-x86 cmake
    export PATH=/tools/cmake/bin:$PATH

*注意*: windows 下建议不使用 msys2 自带的 cmake,而是使用从官网下载的 cmake，
因为交叉编译 android 时， CMAKE_HOST_SYSTEM_NAME 设置为 MSYS ，${ANDROID_NDK}/build/cmake/android.toolchain.cmake
对 CMAKE_HOST_SYSTEM_NAME 处理有 BUG

#### 语法分析工具：bison, flex and gperf（用于Qt编译）

    pacman -S bison flex gperf

#### 本地编译器gcc（版本：5.2.0）：

    pacman -S mingw-w64-i686-gcc

#### 解压缩工具：unzip

    pacman -S unzip

### 编译
#### 启动下列环境之一：
* 启动 msys 环境：

    c:\MinGW\msys\1.0\msys.bat

* 启动 msys2 环境:

    c:\msys32\msys2_shell.cmd -mintty

#### 编译第三方依赖库(脚本中包括qt)

    cd ${RabbitThirdLibraryRoot}/build_script
    ./build.sh android [source_code_directory]

#### 编译本项目:

- 命令行编译：

    $QT_ROOT/bin/qmake THIRD_LIBRARY_PATH= [RabbitCommon_DIR=...]
    make

- 用 QtCreator
  + 打开 QtCreator,并打开项目 RabbitIm.pro
  + 设置参数 THIRD_LIBRARY_PATH=
  + 编译

