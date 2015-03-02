#注意：修改后的本文件不要上传代码库中
#需要设置下面变量：
#export ANDROID_NDK_ROOT=/home/android-ndk-r9c   #指定 android ndk 根目录 
#export ANDROID_NDK=$ANDROID_NDK_ROOT            #指定 android ndk 根目录 
#export ANDROID_SDK=/home/android-sdk/sdk        #指定 android sdk 根目录
#export ANDROID_SDK_ROOT=$ANDROID_SDK   
#export JAVA_HOME=/home/jdk1.7.0_51              #指定 jdk 根目录 
#ANT=/d/software/apache-ant-1.9.4/bin/ant         #ant 程序
QT_ROOT=/home/l/Qt5.3.1/5.3/android_armv7         #QT 安装根目录
JOM=make #/c/Qt/Qt5.3.1/Tools/QtCreator/bin/jom       #设置 QT make 工具 JOM

QT_BIN=${QT_ROOT}/bin       #设置用于 android 平台编译的 qt bin 目录
QMAKE=${QT_BIN}/qmake       #设置用于 unix 平台编译的 QMAKE。
                            #这里设置的是自动编译时的配置，你需要修改为你本地qt编译环境的配置.
                            #注意：修改后的本文件不要上传代码库中

if [ -n "${RabbitImRoot}" ]; then
    PREFIX=${RabbitImRoot}/ThirdLibary/android
else
    PREFIX=`pwd`/../android    #修改这里为安装前缀 
fi

if [ ! -d ${PREFIX} ]; then
    mkdir -p ${PREFIX}
fi

#自动判断主机类型，目前只做了linux、windows判断
TARGET_OS=`uname -s`
case $TARGET_OS in
    MINGW* | CYGWIN*)
        HOST="windows"
        RABBITIM_BUILD_HOST="windows"
        ;;
    Linux* | Unix*)
        HOST="linux-`uname -m`"    #windows、linux-x86_64
        RABBITIM_BUILD_HOST="linux-`uname -m`"    #windows、linux-x86_64
        ;;
    *)
    echo "Please set HOST. see build_android_envsetup.sh"
    return 2
    ;;
esac

TOOLCHAIN_VERSION=4.8   #工具链版本号 
PLATFORMS_VERSION=18    #android api (平台)版本号 
PREBUILT=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-${TOOLCHAIN_VERSION}/prebuilt
PLATFORM=$ANDROID_NDK_ROOT/platforms/android-${PLATFORMS_VERSION}/arch-arm
ANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-$TOOLCHAIN_VERSION
CROSS_PREFIX=$PREBUILT/${HOST}/bin/arm-linux-androideabi-
export PATH=$PREBUILT/${HOST}/bin:$PATH    #把工具链路径加到环境变量中
export PATH=${QT_BIN}:$PATH

echo ""
echo "ANDROID_NDK_ROOT:$ANDROID_NDK_ROOT"
echo "ANDROID_NDK:$ANDROID_NDK_ROOT"
echo "JAVA_HOME:$JAVA_HOME"
echo "QMAKE:$QMAKE"
echo "JOM:$JOM"
echo "PREFIX:$PREFIX"
echo "PREBUILT:$PREBUILT"
echo "PLATFORM:$PLATFORM"
echo "CROSS_PREFIX:$CROSS_PREFIX"
echo "HOST:$HOST"
echo "PATH:$PATH"
echo ""

#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

if [ -n "${RabbitImRoot}" ]; then
    RABBITIM_BUILD_PREFIX=${RabbitImRoot}/ThirdLibary/android
else
    RABBITIM_BUILD_PREFIX=`pwd`/../android    #修改这里为安装前缀 
fi

#自动判断主机类型，目前只做了linux、windows判断
TARGET_OS=`uname -s`
case $TARGET_OS in
    MINGW* | CYGWIN*)
        RABBITIM_BUILD_HOST="windows"
        ;;
    Linux* | Unix*)
        RABBITIM_BUILD_HOST="linux-`uname -m`"    #windows、linux-x86_64
        ;;
    *)
    echo "Please set RABBITIM_BUILD_HOST. see build_android_envsetup.sh"
    return 2
    ;;
esac

RABBITIM_BUILD_TOOLCHAIN_VERSION=4.8   #工具链版本号 
RABBITIM_BUILD_PLATFORMS_VERSION=18 #android api (平台)版本号 
export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:${RABBITIM_BUILD_PREFIX}/lib/pkgconfig

RABBITIM_BUILD_CROSS_PREFIX=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-${RABBITIM_BUILD_TOOLCHAIN_VERSION}/prebuilt/${RABBITIM_BUILD_HOST}/bin/arm-linux-androideabi-
RABBITIM_BUILD_CROSS_SYSROOT=$ANDROID_NDK_ROOT/platforms/android-${PLATFORMS_VERSION}/arch-arm


