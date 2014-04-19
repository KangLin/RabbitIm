PREFIX=`pwd`/../windows
export PATH=$PATH:${PREFIX}/lib/pkgconfig
./configure --prefix=$PREFIX  --enable-static --disable-shared
