PREFIX=`pwd`/../windows

./configure --prefix=$PREFIX \
        --enable-static \
        --enable-pic \
        --disable-cli \
        --disable-opencl \


