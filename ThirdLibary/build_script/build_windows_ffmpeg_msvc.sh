PREFIX=`pwd`/../windows/         #修改这里为安装前缀
./configure  \
    --target-os=win32 \
        --arch=x86 \
    --enable-cross-compile \
        --toolchain=msvc \
    --prefix=$PREFIX \
        --disable-doc \
         --disable-htmlpages \
         --disable-manpages  \
         --disable-podpages \
         --disable-txtpages \
         --disable-ffprobe \
     --disable-ffserver \
         --disable-ffmpeg \
         --disable-ffplay \
        --disable-doc #\
#	 --extra-cflags="-static -I$PREFIX/include" \
#	 --extra-ldflags="-static -L$PREFIX/lib -lx264  " #\
#	  --enable-libx264
