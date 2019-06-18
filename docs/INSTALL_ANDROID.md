编译 ANDROID 平台
================

目录
----

1. [编译脚本与变量](#user-content-编译脚本与变量)
2. [ubuntu下交叉编译](#user-content-ubuntu下交叉编译)
3. [windows下交叉编译](#user-content-windows下交叉编译)

编译脚本与变量
-----------

编译脚本说明：

1. build_envsetup_android.sh : 设置编译时需要的变量  
2. build.sh android [source_code_directory] : 编译第三方库脚本
3. build_XXX.sh android [source_code_directory] : 编译指定的 XXX 第三方库脚本

环境变量说明:

1. export ANDROID_NDK_ROOT=              #android ndk 根目录
2. export ANDROID_NDK=$ANDROID_NDK_ROOT
3. export ANDROID_SDK=                   #android sdk 根目录
4. export ANDROID_SDK_ROOT=$ANDROID_SDK    
5. export JAVA_HOME=                     #jdk根目录 
6. export ANT_ROOT=                      #ant工具的目录
7. ANT=/usr/bin/ant                      #ant 程序
8. QT_ROOT=                              #qt的安装位置,默认为:${RabbitThirdLibraryRoot}/android/qt
9. RABBIT_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`" #make 同时工作进程参数,建议设置为你机器CUP个数
10. RABBIT_BUILD_STATIC="static"       #设置编译静态库，注释掉，则为编译动态库
11. RABBIT_USE_REPOSITORIES="FALSE"    #下载指定的压缩包。省略，则下载开发库。
12. RABBIT_CLEAN=TRUE                  #编译前清理
13. RABBIT_BUILD_TOOLCHAIN_VERSION=4.9   #工具链版本号，默认为4.8
14. ANDROID_NATIVE_API_LEVEL=15    #android ndk api (平台)版本号，默认为9

所需要的环境变量可以保存到系统配置文件 ~/.profile 文件中。作为全局环境变量。但这可能会与其它工程需要的环境变量冲突。
为了避免这个问题。你也可以把环境变量保到 build_envsetup_${RABBIT_BUILD_TARGERT}.sh 文件中。  
再运行:

    cd ${RabbitThirdLibraryRoot}/build_script
    ./build.sh android [source_code_directory] #进行第三方依赖库编译

或者只编译某个库，例如：ffmpeg库

    cd ${RabbitThirdLibraryRoot}/build_script
    ./build_ffmpeg.sh android [source_code_directory] #进行第三方依赖库编译

ubuntu下交叉编译
--------------

### 1. 工具

#### 1.1. java
* 下载：[jdk](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* 设置环境变量：

    export JAVA_HOME=/home/jdk1.7.0_51
    export PATH=$JAVA_HOME/bin:$PATH

#### 1.2. android ndk
* 工具链版本：4.9
* 下载:

    cd /home
    wget https://dl.google.com/android/repository/android-ndk-r17-linux-x86_64.zip
    unzip android-ndk-r17-linux-x86_64.zip
    mv android-ndk-r17 android-ndk


* 设置环境变量

    export ANDROID_NDK=/home/android-ndk
    export ANDROID_NDK_ROOT=$ANDROID_NDK


#### 1.3. android sdk 
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

[git设置](http://blog.csdn.net/kl222/article/details/32903495)

#### 1.9. 脚本工具：
* Perl version 5.12 or later   [http://www.activestate.com/activeperl/]
* Python version 2.7 or later  [http://www.activestate.com/activepython/]
* Ruby version 1.9.3 or later  [http://rubyinstaller.org/]  
    qtwebkit编译需要
* bash:

    sudo apt-get install ruby  build-essential perl python 

#### 1.10. cmake 工具
版本：大于等3.1.0

    cd /home
    wget http://www.cmake.org/files/v3.1/cmake-3.1.0-Linux-x86_64.tar.gz
    tar xf cmake-3.1.0-Linux-x86_64.tar.gz
    mv cmake-3.1.0-Linux-x86_64 cmake
    export PATH=/home/cmake/bin:$PATH

#### 1.11. 语法分析工具：bison, flex and gperf（用于Qt编译）

    sudo apt-get install bison flex gperf

### 2 环境:详见:[INSATLL_UBUNTU.md](INSTALL_UBUNTU.md#user-content-环境)

### 3. 编译第三方依赖库(脚本中包括qt)

    cd ${RabbitThirdLibraryRoot}/build_script
    ./build.sh android [source_code_directory]

### 4. 编译本项目:
1. 命令行编译：

    ```sh
    cd ${RabbitThirdLibraryRoot}/build_script
    ./build_RABBIT.sh android [source_code_directory] [qmake]
    ```
    
第四个参数如果是qmake，则用qmake编译。默认为cmake编译

2. 用 QtCreator  
设置 pkg-config
        
    ```sh
    export PKG_CONFIG_PATH=${RabbitThirdLibraryRoot}/${PLATFORM}         #可选
    export PKG_CONFIG_LIBDIR=${PKG_CONFIG_PATH}                            #可选
    export PKG_CONFIG_SYSROOT_DIR=${RabbitThirdLibraryRoot}/${PLATFORM}  #可选
    ```

windows下交叉编译
---------------

### 1. 环境
#### 1.1. 操作系统:windows 7 旗舰版 版本：6.1 (内部版本 7601 Service Pack 1)
#### 1.2. bash 环境: msys（msys2） 或者 cygwin
* msys: http://www.mingw.org/wiki/MSYS
* msys2: http://sourceforge.net/projects/msys2/
代码位置: https://github.com/Alexpux/MSYS2-packages
* cygwin主页: http://www.cygwin.org/

#### 1.3. 当前文档以msys2为例：
+ 安装
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

+ 启动msys2
  - 启动命令

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

  - 启动

    msys2_shell.cmd -msys2 -mintty

+ 同步：

    pacman -Sy

+ 更新系统：

    pacman -Su

+ 下载工具：

    pacman -S wget subversion git autoconf automake m4 libtool mingw-w64-i686-pkg-config mingw-w64-i686-make bison flex gperf unzip

+ 下载本地编译器gcc（版本：5.2.0）：

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

    vim ~/.bashrc
    export JAVA_HOME="/C/Program Files/Java/jdk1.7.0_51" 
    export PATH=$JAVA_HOME/bin:$PATH

注意：在msys2下需要注意路径符号,用unix型路径："/"

#### 2.2. android ndk
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

#### 2.3. android sdk 
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

#### 2.4. 下载工具
主页： http://www.gnu.org/software/wget/  
一般 msys/cygwin 中会包含

#### 2.5. ant工具
主页： http://ant.apache.org/

    cd /tools
    wget http://apache.dataguru.cn//ant/binaries/apache-ant-1.9.4-bin.zip
    vim ~/.bashrc
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

#### 2.11. 本地编译器gcc（版本：5.2.0）：

    pacman -S mingw-w64-i686-gcc

#### 2.12. 解压缩工具：unzip

    pacman -S unzip

### 3. 编译
#### 3.1 启动下列环境之一：
* 启动 msys 环境：

    c:\MinGW\msys\1.0\msys.bat

* 启动 msys2 环境:

    c:\msys32\msys2_shell.cmd -mintty

#### 3.2. 编译第三方依赖库(脚本中包括qt)

    cd ${RabbitThirdLibraryRoot}/build_script
    ./build.sh android [source_code_directory]

#### 3.3. 编译本项目:
1. 命令行编译：

    ```sh
    cd ${RabbitThirdLibraryRoot}/build_script
    ./build_RABBIT.sh android [source_code_directory] [qmake]
    ```
    
第四个参数如果是qmake，则用qmake编译。默认为cmake编译

2. 用 QtCreator  
  设置 pkg-config
        
    ```sh
    export PKG_CONFIG=${MSYS32_ROOT}/mingw32/bin/pkg-config
    export PKG_CONFIG_PATH=${RabbitThirdLibraryRoot}/${PLATFORM}        #可选
    export PKG_CONFIG_LIBDIR=${PKG_CONFIG_PATH}                         #可选
    export PKG_CONFIG_SYSROOT_DIR=${RabbitThirdLibraryRoot}/${PLATFORM} #可选
    ```

  设置 GIT
  
  ```sh
  export GIT=${MSYS2_ROOT}/bin/git.exe
  ```
  
### 4. 调试
下载 ministro  
https://necessitas.kde.org/necessitas/ministro.php
