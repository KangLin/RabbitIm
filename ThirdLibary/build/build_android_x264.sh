NDK=$(ANDROID_ROOT)
PREFIX=`pwd`/../android/
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


