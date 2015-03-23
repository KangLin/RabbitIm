## 目录
1. [ubuntu下交叉编译](#user-content-ubuntu下交叉编译)

2. [windows下交叉编译](#user-content-windows下交叉编译)

## ubuntu下交叉编译
### 1. 环境:详见:[INSATLL_UBUNTU.md](INSTALL_UBUNTU.md#user-content-环境)
### 2. java
* 下载：    
* 设置环境变量：

        export JAVA_HOME=/home/jdk1.7.0_51
        export PATH=$JAVA_HOME/bin:$PATH:$ANDROID_SDK/platform-tools

### 3. android ndk
* 工具链版本：4.8
* 下载:

        cd /home
        wget http://dl.google.com/android/ndk/android-ndk-r9c-linux-x86_64.tar.bz2
        tar xf android-ndk-r9c-linux-x86_64.tar.bz2
        mv android-ndk-r9c android-ndk

* 设置环境变量

    
        export ANDROID_NDK=/home/android-ndk
        export ANDROID_NDK_ROOT=$ANDROID_NDK
    

### 3. android sdk 
* 平台版本：18
* 下载：

        wget http://182.254.185.29/download/travis/android-sdk.tar.gz 
        tar xzf android-sdk.tar.gz 
        mv sdk android-sdk
    
* 设置环境变量
    
        export ANDROID_SDK=/home/android-sdk/sdk
        export ANDROID_SDK_ROOT=$ANDROID_SDK

### 4. 安装32位库:android sdk需要 :

    sudo apt-get install zlib1g:i386 libstdc++6:i386 libc6:i386 -y -qq

### 5. 安装工具:参见[INSATLL_UBUNTU.md](INSATLL_UBUNTU.md#user-content-工具)

### 6. 编译第三方依赖库(脚本中包括qt)

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build.sh android [source_code_directory]

## windows下交叉编译
### 1. java
* 下载：
    
* 设置环境变量：
    
        export JAVA_HOME=/home/jdk1.7.0_51
        export PATH=$JAVA_HOME/bin:$PATH:$ANDROID_SDK/platform-tools

### 2. android ndk
* 工具链版本：4.8
* 下载:
    
        cd /home
        wget http://dl.google.com/android/ndk/android-ndk-r9c-linux-x86_64.tar.bz2
        tar xf android-ndk-r9c-linux-x86_64.tar.bz2
        mv android-ndk-r9c android-ndk

* 设置环境变量
    
        export ANDROID_NDK=/home/android-ndk
        export ANDROID_NDK_ROOT=$ANDROID_NDK
    
### 3. android sdk 
* 平台版本：18
* 下载：

        wget http://182.254.185.29/download/travis/android-sdk.tar.gz 
        tar xzf android-sdk.tar.gz 
        mv sdk android-sdk

* 设置环境变量
    
        export ANDROID_SDK=/home/android-sdk/sdk
        export ANDROID_SDK_ROOT=$ANDROID_SDK

### 4. 安装32位库:android sdk需要 :

    sudo apt-get install zlib1g:i386 libstdc++6:i386 libc6:i386 -y -qq

### 5. 安装工具:详见[INSTALL_WINDOWS.md](INSTALL_WINDOWS.md)

### 6. 编译第三方依赖库(脚本中包括qt)

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build.sh android [source_code_directory]
    
### 7. 编译本工程:

    cd ${RabbitImRoot}/ThirdLibary/build_script
    ./build_rabbitim.sh android [source_code_directory]
