#需要设置下面变量：
#ANDROID_NDK_ROOT=/e/source/android/android-ndk-r9               #指定 android ndk 根目录 
#ANDROID_NDK=$ANDROID_NDK_ROOT  #指定 android ndk 根目录 
#ANDROID_SDK=/e/source/android/android-sdk-windows                   #指定 android sdk 根目录
#ANDROID_SDK_ROOT=$ANDROID_SDK   
#JAVA_HOME=                      #指定 jdk 根目录 
#QMAKE=/c/Qt/Qt5.3.0_android/5.3/android_armv7/bin/qmake.exe   #设置用于 mingw 平台编译的 QMAKE
#JOM=/c/Qt/Qt5.3.0_android/Tools/QtCreator/bin/jom.exe                 #设置 QT make 工具 JOM

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
        ;;
    Linux* | Unix*)
        HOST="linux-`uname -m`"    #windows、linux-x86_64
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
CROSS_PREFIX=$PREBUILT/${HOST}/bin/arm-linux-androideabi-
export PATH=$PREBUILT/${HOST}/bin:$PATH    #把工具链路径加到环境变量中

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
