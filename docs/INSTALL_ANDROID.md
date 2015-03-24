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
* python:2.7.6
* perl:5.18.2
* bash:
* ruby:qtwebkit编译需要

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

### 1. 工具
#### 1.1 java
* 下载：[jdk](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
* 设置环境变量：

        export JAVA_HOME=/home/jdk1.7.0_51
        export PATH=$JAVA_HOME/bin:$PATH:$ANDROID_SDK/platform-tools

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

        wget http://182.254.185.29/download/travis/android-sdk.tar.gz 
        tar xzf android-sdk.tar.gz 
        mv sdk android-sdk

* 设置环境变量
    
        export ANDROID_SDK=/home/android-sdk/sdk
        export ANDROID_SDK_ROOT=$ANDROID_SDK

### 2. 环境

### 3. 第三方依赖库
#### 3.1. 编译第三方依赖库(脚本中包括qt)

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build.sh android [source_code_directory]
    
### 4. 编译本项目:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh android [source_code_directory] [qmake]

第四个参数如果是qmake，则用qmake编译。默认为cmake编译