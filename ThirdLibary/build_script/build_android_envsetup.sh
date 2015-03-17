#注意：修改后的本文件不要上传代码库中
#需要设置下面变量：
#export ANDROID_NDK_ROOT=/home/android-ndk-r9c   #指定 android ndk 根目录 
#export ANDROID_NDK=$ANDROID_NDK_ROOT            #指定 android ndk 根目录 
#export ANDROID_SDK=/home/android-sdk/sdk        #指定 android sdk 根目录
#export ANDROID_SDK_ROOT=$ANDROID_SDK   
#export JAVA_HOME=/home/jdk1.7.0_51              #指定 jdk 根目录 

ANT=/usr/bin/ant         #ant 程序
#QT_ROOT=/usr/local/Qt-5.5.0-android         #QT 安装根目录
JOM=make #/c/Qt/Qt5.3.1/Tools/QtCreator/bin/jom       #设置 QT make 工具 JOM

if [ -z "$ANDROID_NDK_ROOT" -o -z "$ANDROID_NDK" -o -z "$ANDROID_SDK" -o -z "$ANDROID_SDK_ROOT"	-o -z "$JAVA_HOME" ]; then
	echo "Please set ANDROID_NDK_ROOT and ANDROID_NDK and ANDROID_SDK and ANDROID_SDK_ROOT and JAVA_HOME"
	return 1 
fi

#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

if [ -n "${RabbitImRoot}" ]; then
    RABBITIM_BUILD_PREFIX=${RabbitImRoot}/ThirdLibary/android
else
    RABBITIM_BUILD_PREFIX=`pwd`/../android    #修改这里为安装前缀 
fi
if [ ! -d ${RABBITIM_BUILD_PREFIX} ]; then
    mkdir -p ${RABBITIM_BUILD_PREFIX}
fi

if [ -z "$QT_ROOT" ]; then
        QT_ROOT=${RABBITIM_BUILD_PREFIX}/qt
fi
QT_BIN=${QT_ROOT}/bin       #设置用于 android 平台编译的 qt bin 目录
QMAKE=${QT_BIN}/qmake       #设置用于 unix 平台编译的 QMAKE。
                            #这里设置的是自动编译时的配置，你需要修改为你本地qt编译环境的配置.

#自动判断主机类型，目前只做了linux、windows判断
TARGET_OS=`uname -s`
case $TARGET_OS in
    MINGW* | CYGWIN*)
        RABBITIM_BUILD_HOST="windows"
        RABBITIM_CMAKE_MAKE_PROGRAM=$ANDROID_NDK/prebuilt/${RABBITIM_BUILD_HOST}/bin/make
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

RABBITIM_BUILD_CROSS_PREFIX=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-${RABBITIM_BUILD_TOOLCHAIN_VERSION}/prebuilt/${RABBITIM_BUILD_HOST}/bin/arm-linux-androideabi-
RABBITIM_BUILD_CROSS_SYSROOT=$ANDROID_NDK_ROOT/platforms/android-${RABBITIM_BUILD_PLATFORMS_VERSION}/arch-arm
RABBITIM_BUILD_CROSS_HOST=arm-linux-androideabi
export ANDROID_API_VERSION=android-${RABBITIM_BUILD_PLATFORMS_VERSION}
export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:${RABBITIM_BUILD_PREFIX}/lib/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=${RABBITIM_BUILD_PREFIX}
export PATH=${QT_BIN}:$PATH
