PREFIX=`pwd`/../windows/         #修改这里为安装前缀

./configure --disable-shared \
    --enable-static \
    --enable-static-msvcrt \
    --disable-docs --disable-examples \
    --target=x86-win32-vs12 \
    --prefix=$PREFIX

make install
