编译 unix 平台
=============

目录
----

1. [环境](#user-content-环境)
2. [工具](#user-content-工具)
3. [第三方库](#user-content-第三方库)
4. [本项目录编译](#user-content-本项目录编译)

## 环境

### 1. 当前ubuntu使用版本:

    $lsb_release -a
    No LSB modules are available.
    Description:    Ubuntu 16.04.6 LTS
    Release:        16.04
    Codename:       xenial

### 2. 自动编译ubuntu使用版本:

    $ lsb_release -a
    No LSB modules are available.
    Distributor ID: Ubuntu
    Description:    Ubuntu 12.04.4 LTS
    Release:    12.04
    Codename:   precise

[自动编译](https://travis-ci.org/KangLin/RabbitThirdLibrary)

## 工具

### 1. 编译工具：gcc/g++ 当前使用版本(4.8.2)

    sudo apt-get install gcc g++

### 2. auto工具：

    sudo apt-get install autoconf automake libtool m4 make
    sudo apt-get install pkg-config

### 3. 版本管理工具：

    sudo apt-get install subversion git

[git设置](http://blog.csdn.net/kl222/article/details/32903495)

### 4. 脚本工具：
* python:2.7.6
* perl:5.18.2
* bash:
* ruby:qtwebkit编译需要

    sudo apt-get install ruby build-essential perl python 

### 5. 汇编工具：

    sudo apt-get install yasm

### 6. 安装工具：

    sudo apt-get install dh-make debhelper

### 7. Qt工具：
- QT工具包括：
  * qmake
  * uic
  * rcc
  * lrelease
  * lupdate
- 安装QT（可选）

        #See: https://launchpad.net/~beineri
        sudo add-apt-repository ppa:beineri/opt-qt-5.12.0-xenial
        sudo apt-get update
        sudo apt-get install -y -qq qt512base qt512tools qt512multimedia

- 从官网安装

        http://download.qt.io/official_releases/qt/

### 8. 语法分析工具：bison, flex and gperf（用于Qt编译）

    sudo apt-get install bison flex gperf

### 9. icu：编译 QtWebkit 需要

    sudo apt-get install icu-devtools

### 10. 下载工具

    sudo apt-get install wget

### 11. cmake 工具

    cd /home
    wget http://www.cmake.org/files/v3.1/cmake-3.1.0-Linux-x86_64.tar.gz
    tar xf cmake-3.1.0-Linux-x86_64.tar.gz
    mv cmake-3.1.0-Linux-x86_64 cmake
    export PATH=/home/cmake/bin:$PATH

## 第三方库

### 1. 安装第三方库开发包

#### 1.1. 编解码库(libvpx)安装：

    sudo apt-get install libvpx-dev

#### 1.2. ffmpeg安装：

    sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev

#### 1.3. opencv安装：

    sudo apt-get install libopencv-dev

安装opencv时，会自动安装ffmpeg库
    
#### 1.4. libspeex 安装:

    sudo apt-get install libspeex-dev libspeexdsp-dev 

#### 1.5. openssl:

    sudo apt-get install libssl-dev 

#### 1.6. libcurl:

    sudo apt-get install libcurl4-openssl-dev
    
#### 1.7 libqrencode

     sudo apt-get install libqrencode-dev 

#### 1.8 libqxmpp

     sudo apt-get install libqxmpp-dev 

#### 1.8. 下载预编译第三方库:

从　https://github.com/KangLin/RabbitThirdLibrary/releases　下载linux操作系统的预编译的第三方库，并解压到　${RABBITRoot}/ThirdLibrary　下。

    cd ${RABBITRoot}/ThirdLibrary
    7z x ThirdLibrary_linux.7z 

修改前缀

    cd ${RABBITRoot}/ThirdLibrary/unix
    cp ${RABBITRoot}/ThirdLibrary/build_script/change_prefix.sh ${RABBITRoot}/ThirdLibrary/unix/change_prefix.sh
    cd ${RABBITRoot}/ThirdLibrary/unix
    ./change_prefix ${old_prefix} ${new_prefix}  #old_prefix是原来的前缀，你可打开一个配置文件查看；
                                                 #new_prefix可以省略，省略时用当前目录

### 2. 编译第三方依赖库(脚本中包括qt)

如果没有你主机操作系统相同的预编译第三方库，或者你用着出错时，那你需要自己编译第三方依赖库。

编译脚本：

1. build.sh unix [source_code_directory] : 编译第三方库脚本
2. build_envsetup_unix.sh: 设置编译时需要的变量
3. build_XXX.sh unix [source_code_directory] : 编译指定的 XXX 第三方库脚本

环境变量说明： 

1. QT_ROOT=                          #qt的安装位置,默认为:${RABBITRoot}/ThirdLibrary/unix/qt
2. RABBIT_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数,建议设置为你机器CUP个数
3. RABBIT_BUILD_STATIC="static"    #设置编译静态库，注释掉，则为编译动态库
4. RABBIT_USE_REPOSITORIES="FALSE" #下载指定的压缩包。省略，则下载开发库。
5. RABBIT_CLEAN=TRUE               #编译前清理

编译所有依赖的第三方库：

    cd ${RABBITRoot}/ThirdLibrary/build_script
    ./build.sh unix [source_code_directory]

或者只编译其中一个库，例如：libvpx

    cd ${RABBITRoot}/ThirdLibrary/build_script
    ./build_libvpx.sh unix [source_code_directory]

#### 2.1. qt 编译：
* openssl:
* Libxcb:

    sudo apt-get install "^libxcb.*" libx11-xcb-dev libxrender-dev libxi-dev

* libicu：（qtwebkit需要）

    sudo apt-get install libicu-dev

* opengl

    sudo apt-get install build-essential
    sudo apt-get install mesa-common-dev libglu1-mesa-dev

* libasound2、libgstreamer

    sudo apt-get install libasound2-dev
    sudo apt-get install libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev

* libfontconfig (需要这个库才能用系统字体，会调用系统 freetype,否则会调用qt自己的freetype。qt自己的freetype字体不含中文)

    sudo apt-get install libfontconfig1-dev 

* 编译

    cd ${RABBITRoot}/ThirdLibrary/build_script
    ./build_qt.sh unix [source_code_directory]

#### 2.2. qxmpp:

    cd ${RABBITRoot}/ThirdLibrary/build_script
    ./build_qxmpp.sh unix [source_code_directory]
