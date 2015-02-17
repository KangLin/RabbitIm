#不能支持
PREFIX=`pwd`/../windows

./configure --prefix=$PREFIX \
        --enable-pic \
        --disable-cli \
        --disable-opencl \
        --enable-static 


