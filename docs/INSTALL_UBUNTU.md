## 目录
1. [环境](#user-content-环境)

2. [工具](#user-content-工具)

3. [第三方库](#user-content-第三方库)

## 环境

1. 当前使用版本:

    $lsb_release -a
    No LSB modules are available.
    Distributor ID: Ubuntu
    Description:    Ubuntu 14.04.2 LTS
    Release:    14.04
    Codename:   trusty

## 工具

1. 编译工具：gcc/g++ 当前使用版本：4.8.2 

    sudo apt-get install gcc g++

2. auto工具：

    sudo apt-get install autoconf automake libtool
    sudo apt-get install pkg-config

3. 版本管理工具：

    sudo apt-get install subversion git

4. 脚本工具：
* python:2.7.6
* perl:5.18.2
* bash:
* ruby:qtwebkit编译需要

    sudo apt-get install ruby 

5. 汇编工具：

    sudo apt-get install yasm

6. 安装工具：

    sudo apt-get install dh-make debhelper
    
7. Qt工具：

* qmake
* uic
* rcc
* lrelease
* lupdate

8. 语法分析工具：bison, flex and gperf（用于Qt编译）

    sudo apt-get install bison flex gperf

9. icu：编译 QtWebkit 需要

    sudo apt-get install icu-devtools
    
10. ant工具

    sudo apt-get install ant


## 第三方库

1. 编解码库(libvpx)安装：

    sudo apt-get install libvpx-dev

2. ffmpeg安装：

    sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev

3. opencv安装：

    sudo apt-get install libopencv-dev

    安装opencv时，会自动安装ffmpeg库
    
4. libspeex 安装:

    sudo apt-get install libspeex-dev libspeexdsp-dev 

5. openssl:

    sudo apt-get install libssl-dev 

6. libcurl:

    sudo apt-get install libcurl4-openssl-dev

7. libicu:

    sudo apt-get install libicu-dev

8. qt 编译：

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_qt.sh unix [source_code_directory]

9. qxmpp:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_qxmpp.sh unix [source_code_directory]
    
10. rabbitim:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh unix [source_code_directory]

