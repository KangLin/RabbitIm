NDK=$(ANDROID_NDK)               #指定android ndk根目录
PREFIX=`pwd`/../android/         #修改这里为安装前缀
PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.8/prebuilt
PLATFORM=$NDK/platforms/android-18/arch-arm
CROSS_PREFIX=$PREBUILT/windows/bin/arm-linux-androideabi-

export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
../configure --prefix=$PREFIX  --enable-static --disable-shared  \
    CC=${CROSS_PREFIX}gcc \
    --disable-shared \
    --enable-static \
    CFLAGS="-march=armv7-a -mfpu=neon --sysroot=${PLATFORM}" \
    CPPFLAGS="-march=armv7-a -mfpu=neon --sysroot=${PLATFORM}" \
    --host=arm-linux-androideabi \
