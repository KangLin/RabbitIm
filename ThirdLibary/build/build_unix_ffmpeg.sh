PREFIX=`pwd`/../unix        #修改这里为安装前缀

./configure --enable-libx264 --enable-gpl  \
                    --disable-ffplay --disable-ffserver \
                    --disable-ffmpeg --disable-ffprobe \
                    --disable-doc  \
                    --prefix=$PREFIX \
                    --extra-ldflags='-L$PREFIX/lib -lx264 -static'  \
                    --extra-cflags='-I$PREFIX/include -static'
