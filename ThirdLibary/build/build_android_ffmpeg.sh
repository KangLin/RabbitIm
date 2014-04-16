NDK=$(ANDROID_NDK)               #指定android ndk根目录
PREFIX=`pwd`/../android/         #修改这里为安装前缀
PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.8/prebuilt
PLATFORM=$NDK/platforms/android-18/arch-arm
CROSS_PREFIX=$PREBUILT/windows/bin/arm-linux-androideabi-

./configure  --target-os=linux --prefix=$PREFIX \
	--enable-cross-compile \
	--enable-runtime-cpudetect \
	--disable-doc \
	 --disable-htmlpages \
	 --disable-manpages  \
	 --disable-podpages \
	 --disable-txtpages \
	 --disable-ffprobe \
	  --disable-ffserver \
	--arch=arm \
	--cpu=armv7-a \
	--cross-prefix=$CROSS_PREFIX \
	--sysroot=$PLATFORM \
	--disable-doc \
	 --extra-cflags="-march=armv7-a -mfpu=neon -I$PREFIX/include " \
	 --extra-ldflags="-L$PREFIX/lib -lx264" \
	 --enable-gpl --enable-neon --enable-libx264
