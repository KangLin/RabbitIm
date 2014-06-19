NDK=$(ANDROID_NDK_ROOT)               #指定android ndk根目录
PREFIX=`pwd`/../android/         #修改这里为安装前缀
PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.8/prebuilt
PLATFORM=$NDK/platforms/android-18/arch-arm
CROSS_PREFIX=$PREBUILT/windows/bin/arm-linux-androideabi-

./configure  --prefix=$PREFIX \
    CC=${CROSS_PREFIX}gcc \
    --disable-shared \
    --disable-oggtest \
    --enable-vbr \
    CFLAGS="-march=armv7-a -mfpu=neon --sysroot=${PLATFORM}" \
    CPPFLAGS="-march=armv7-a -mfpu=neon --sysroot=${PLATFORM}" \
    --host=arm-linux-androideabi \
    SPEEXDSP_CFLAGS=-I$PREFIX/include SPEEXDSP_LIBS=-L$PREFIX/lib
