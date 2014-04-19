PREFIX=`pwd`/../windows

export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
./configure --prefix=$PREFIX  --enable-static \
    --disable-shared --disable-oggtest \
    SPEEXDSP_CFLAGS=-I$PREFIX/include \
    SPEEXDSP_LIBS=-L$PREFIX/lib
