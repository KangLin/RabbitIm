NDK=$(ANDROID_NDK_ROOT)
PREFIX=`pwd`/../android

./configure --target=armv7-android-gcc \
    --sdk-path=${NDK} \
    --prefix=${PREFIX} \
    --disable-examples  \
    --disable-install-docs \
    --disable-unit-tests \
    --extra-cflags="-mfloat-abi=softfp -mfpu=neon " \
    --disable-debug  \
    --disable-debug-libs \
    --disable-shared \
    --enable-static 

make install
