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
    Distributor ID: Ubuntu
    Description:    Ubuntu 14.04.2 LTS
    Release:    14.04
    Codename:   trusty

### 2. 自动编译ubuntu使用版本:

    $ lsb_release -a
    No LSB modules are available.
    Distributor ID: Ubuntu
    Description:    Ubuntu 12.04.4 LTS
    Release:    12.04
    Codename:   precise

[自动编译](https://travis-ci.org/KangLin/rabbitim)

## 工具

### 1. 编译工具：gcc/g++ 当前使用版本(4.8.2)

    sudo apt-get install gcc g++

### 2. auto工具：

    sudo apt-get install autoconf automake libtool m4
    sudo apt-get install pkg-config

### 3. 版本管理工具：

    sudo apt-get install subversion git

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

* qmake
* uic
* rcc
* lrelease
* lupdate

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

### 1. 编解码库(libvpx)安装：

    sudo apt-get install libvpx-dev

### 2. ffmpeg安装：

    sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev

### 3. opencv安装：

    sudo apt-get install libopencv-dev

安装opencv时，会自动安装ffmpeg库
    
### 4. libspeex 安装:

    sudo apt-get install libspeex-dev libspeexdsp-dev 

### 5. openssl:

    sudo apt-get install libssl-dev 

### 6. libcurl:

    sudo apt-get install libcurl4-openssl-dev

### 7. qt 编译：
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

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_qt.sh unix [source_code_directory]

### 8. qxmpp:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_qxmpp.sh unix [source_code_directory]

## 本项目录编译

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh unix [source_code_directory] [qmake]

第四个参数如果是qmake，则用qmake编译。默认为cmake编译
