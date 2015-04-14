编译 ANDROID 平台
================

目录
----

1. [ubuntu下交叉编译](#user-content-ubuntu下交叉编译)

2. [windows下交叉编译](#user-content-windows下交叉编译)

ubuntu下交叉编译
--------------

### 1. 工具

#### 1.1. java
* 下载：[jdk](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* 设置环境变量：

    export JAVA_HOME=/home/jdk1.7.0_51
    export PATH=$JAVA_HOME/bin:$PATH

#### 1.2. android ndk
* 工具链版本：4.8
* 下载:

    cd /home
    wget http://dl.google.com/android/ndk/android-ndk-r9c-linux-x86_64.tar.bz2
    tar xf android-ndk-r9c-linux-x86_64.tar.bz2
    mv android-ndk-r9c android-ndk

* 设置环境变量

    export ANDROID_NDK=/home/android-ndk
    export ANDROID_NDK_ROOT=$ANDROID_NDK

#### 1.3. android sdk 
* 平台版本：18
* 下载：

    cd /home
    wget http://182.254.185.29/download/travis/android-sdk.tar.gz
    tar xzf android-sdk.tar.gz
    mv sdk android-sdk

* 设置环境变量
    
    export ANDROID_SDK=/home/android-sdk/sdk
    export ANDROID_SDK_ROOT=$ANDROID_SDK
    export PATH=$PATH:$ANDROID_SDK/platform-tools

#### 1.4. 安装32位库(可选): ubuntu 64位下 android sdk 需要

    sudo apt-get install zlib1g:i386 libstdc++6:i386 libc6:i386
    
#### 1.5. ant工具

    sudo apt-get install ant

#### 1.6. 下载工具

    sudo apt-get install wget

#### 1.7. auto工具：

    sudo apt-get install autoconf automake libtool m4
    sudo apt-get install pkg-config

#### 1.8. 版本管理工具：

    sudo apt-get install subversion git

#### 1.9. 脚本工具：
* Perl version 5.12 or later   [http://www.activestate.com/activeperl/]
* Python version 2.7 or later  [http://www.activestate.com/activepython/]
* Ruby version 1.9.3 or later  [http://rubyinstaller.org/]  
    qtwebkit编译需要
* bash:

    sudo apt-get install ruby  build-essential perl python 

#### 1.10. cmake 工具

    cd /home
    wget http://www.cmake.org/files/v3.1/cmake-3.1.0-Linux-x86_64.tar.gz
    tar xf cmake-3.1.0-Linux-x86_64.tar.gz
    mv cmake-3.1.0-Linux-x86_64 cmake
    export PATH=/home/cmake/bin:$PATH

#### 1.11. 语法分析工具：bison, flex and gperf（用于Qt编译）

    sudo apt-get install bison flex gperf

### 2 环境:详见:[INSATLL_UBUNTU.md](INSTALL_UBUNTU.md#user-content-环境)

### 3. 编译第三方依赖库(脚本中包括qt)

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build.sh android [source_code_directory]

### 4. 编译本项目:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh android [source_code_directory] [qmake]

第四个参数如果是qmake，则用qmake编译。默认为cmake编译

windows下交叉编译
---------------

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

    pacman -S wget subversion git autoconf automake m4 libtool pkg-config make bison flex gperf

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

### 2. 工具
#### 2.1 java
* 下载：[jdk](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* 设置环境变量：

    export JAVA_HOME=/home/jdk1.7.0_51
    export PATH=$JAVA_HOME/bin:$PATH

#### 2.2. android ndk
* 工具链版本：4.8
* 下载:

    cd /home
    wget http://dl.google.com/android/ndk/android-ndk-r9c-linux-x86_64.tar.bz2
    tar xf android-ndk-r9c-linux-x86_64.tar.bz2
    mv android-ndk-r9c android-ndk

* 设置环境变量

    export ANDROID_NDK=/home/android-ndk
    export ANDROID_NDK_ROOT=$ANDROID_NDK
    export PATH=$PATH::$ANDROID_SDK/platform-tools

#### 2.3. android sdk 
* 平台版本：18
* 下载：

    wget http://182.254.185.29/download/travis/android-sdk.tar.gz 
    tar xzf android-sdk.tar.gz 
    mv sdk android-sdk

* 设置环境变量

    cd /home
    export ANDROID_SDK=/home/android-sdk/sdk
    export ANDROID_SDK_ROOT=$ANDROID_SDK

#### 2.4. 下载工具
主页： http://www.gnu.org/software/wget/  
一般 msys/cygwin 中会包含

#### 2.5. ant工具
主页： http://ant.apache.org/

    cd /tools
    wget http://apache.dataguru.cn//ant/binaries/apache-ant-1.9.4-bin.zip
    #并把路径加到环境变量PATH中
    export PATH=/tools/ant/bin:$PATH

#### 2.6 auto工具：
主页:
* automake: http://www.gnu.org/software/automake/
* autoconf: http://www.gnu.org/software/autoconf/
* libtool: http://www.gnu.org/software/libtool/
* m4: http://www.gnu.org/software/m4
* pkg-config: http://www.freedesktop.org/wiki/Software/pkg-config/
* make:当前用的是msys make

一般 msys/cygwin 中会包含

#### 2.7. 版本管理工具：
* subversion: http://subversion.apache.org/
* git: http://www.git-scm.com/

需要把路径加到环境变量 PATH 中

#### 2.8. 脚本工具：
* python:2.7.6  version 2.7 or later
主页: https://www.python.org/  http://www.activestate.com/activepython/
* perl：5.18.2  version 5.12 or later 
主页： http://www.perl.org/  http://www.activestate.com/activeperl/
* bash:msys或cygwin
* ruby:qtwebkit编译需要 version 1.9.3 or later  
主页：http://www.ruby-lang.org/  http://rubyinstaller.org/  
https://github.com/ruby/ruby

#### 2.9. cmake 工具

    cd /tools
    wget http://www.cmake.org/files/v3.1/cmake-3.1.0-rc1-win32-x86.zip
    unxip cmake-3.1.0-rc1-win32-x86.zip
    mv cmake-3.1.0-rc1-win32-x86 cmake
    export PATH=/tools/cmake/bin:$PATH

    pacman -S mingw-w64-i686-cmake

#### 2.10. 语法分析工具：bison, flex and gperf（用于Qt编译）

    pacman -S bison flex gperf

#### 2.11. 本地编译器gcc（版本：4.9.2）：

    pacman -S mingw-w64-i686-gcc

### 3. 编译
#### 3.1 启动下列环境之一：
* 启动 msys 环境：

    c:\MinGW\msys\1.0\msys.bat

* 启动 msys2 环境:

    c:\msys32\msys2_shell.bat

#### 3.2. 编译第三方依赖库(脚本中包括qt)

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build.sh android [source_code_directory]

#### 3.3. 编译本项目:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh android [source_code_directory] [qmake]

第四个参数如果是qmake，则用qmake编译。默认为cmake编译
