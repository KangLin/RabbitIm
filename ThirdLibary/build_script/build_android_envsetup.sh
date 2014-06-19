#ANDROID_NDK_ROOT #指定 android ndk 根目录 
#ANDROID_NDK      #指定 android ndk 根目录 
#JAVA_HOME        #指定 jdk 根目录 
if [ -n "${RabbitImRoot}" ]; then
    PREFIX=${RabbitImRoot}/ThirdLibary/android
else
    PREFIX=`pwd`/../android    #修改这里为安装前缀 
fi

TARGET_OS=`uname -s`
if [ "$TARGET_OS" = "Windows" ]; then
    HOST=windows
elif [ "$TARGET_OS" = "Linux" ]; then
        HOST=linux-`uname -m`    #windows、linux-x86_64
else
    echo "please set HOST. see build_android_envsetup.sh"
fi

TOOLCHAIN_VERSION=4.8   #工具链版本号 
PLATFORMS_VERSION=18    #android api (平台)版本号 
PREBUILT=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-${TOOLCHAIN_VERSION}/prebuilt
PLATFORM=$ANDROID_NDK_ROOT/platforms/android-${PLATFORMS_VERSION}/arch-arm
CROSS_PREFIX=$PREBUILT/${HOST}/bin/arm-linux-androideabi-

echo ""
echo "ANDROID_NDK_ROOT:$ANDROID_NDK_ROOT"
echo "ANDROID_NDK:$ANDROID_NDK_ROOT"
echo "JAVA_HOME:$JAVA_HOME"
echo "PREFIX:$PREFIX"
echo "PREBUILT:$PREBUILT"
echo "PLATFORM:$PLATFORM"
echo "CROSS_PREFIX:$CROSS_PREFIX"
echo ""
