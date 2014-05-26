PREFIX=`pwd`/../windows/         #修改这里为安装前缀
SDK=                             #dshow库的位置，它只用于dshow摄像头捕获
./configure  \
    --prefix=$PREFIX \
    --enable-gpl --enable-runtime-cpudetect \
    --disable-doc \
    --disable-htmlpages \
    --disable-manpages  \
    --disable-podpages \
    --disable-txtpages \
    --disable-ffprobe \
    --disable-ffserver \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-doc \
    --extra-cflags="-static -I$PREFIX/include -I$SDK" \
    --extra-ldflags="-static -L$PREFIX/lib -lx264 -lspeex -lvpx" \
    --enable-libx264 \
    --enable-libvpx \
    --enable-libspeex

