NDK=$(ANDROID_ROOT)
PREFIX=`pwd`/../windows

./configure --target=armv7-android-gcc \
    --sdk-path=${NDK} \
    --prefix=${PREFIX} \
    --disable-examples  \
    --disable-install-docs \
    --disable-unit-tests

make install
