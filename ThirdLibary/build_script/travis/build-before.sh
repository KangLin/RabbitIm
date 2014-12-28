set -ev
SOURCE_DIR="`pwd`"
cd ThirdLibary/build_script
wget http://182.254.185.29/download/qt.tar.gz
tar xzvf qt.tar.gz 
mv gcc_64 qt

echo `pwd`/qt/bin/qmake

source build_unix.sh 

sed -i "s/#define PixelFormat AVPixelFormat/\/\/#define PixelFormat AVPixelFormat/g" ../unix/include/libavutil/pixfmt.h
cd ${SOURCE_DIR}