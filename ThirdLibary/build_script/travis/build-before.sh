#下载编译好的第三方依赖库

set -ev

SOURCE_DIR="`pwd`"
cd ThirdLibary

if [ -n "$ANDROID_TARGET" ]; then
    #下载android ndk
    wget http://dl.google.com/android/ndk/android-ndk-r9c-linux-x86_64.tar.bz2
    tar xf android-ndk-r9c-linux-x86_64.tar.bz2
    mv android-ndk-r9c android-ndk

    #wget https://dl.google.com/android/ndk/android-ndk-r10d-linux-x86_64.bin
    #chmod a+x android-ndk-r10d-linux-x86_64.bin
    #./android-ndk-r10d-linux-x86_64.bin > /dev/null
    #mv android-ndk-r10d android-ndk
    export ANDROID_NDK_ROOT=`pwd`/android-ndk
    
    #下载qt for android
    wget http://182.254.185.29/download/travis/qt_android.tar.gz
    tar xzf qt_android.tar.gz 
    mv android_armv7 qt
    
    #下载第三方依赖库
    wget http://182.254.185.29/download/travis/android.tar.gz
    tar xzf android.tar.gz 
else
   #下载qt for linux
    wget http://182.254.185.29/download/travis/qt_linux.tar.gz
    tar xzf qt_linux.tar.gz 
    mv gcc_64 qt
    
    #下载第三方依赖库
    wget http://182.254.185.29/download/travis/unix.tar.gz
    tar xzf unix.tar.gz
fi

#下载最新cmake程序
if [ -n "$CMAKE" ]; then
    wget http://www.cmake.org/files/v3.1/cmake-3.1.0-Linux-x86_64.tar.gz
    tar xf cmake-3.1.0-Linux-x86_64.tar.gz
fi

cd ${SOURCE_DIR}
