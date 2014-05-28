NDK=$(ANDROID_NDK)               #指定android ndk根目录
PREFIX=`pwd`/../android/         #修改这里为安装前缀
NDK=/e/source/android/android-ndk-r9
PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.8/prebuilt
PLATFORM=$NDK/platforms/android-18/arch-arm
CROSS_PREFIX=$PREBUILT/windows/bin/arm-linux-androideabi-

./configure --prefix=$PREFIX \
    --enable-static \
    --enable-pic \
    --disable-cli \
    --host=arm-linux \
    --cross-prefix=$CROSS_PREFIX \
    --sysroot=$PLATFORM \
    --extra-cflags="-march=armv7-a -mfpu=neon" \
    --extra-asflags="-march=armv7-a -mfpu=neon"
