#参数:
#    $1:源码的位置 

#运行本脚本前,先运行 build_windows_mingw_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   PREFIX=`pwd`/../windows  #修改这里为安装前缀
if [ -z "${PREFIX}" ]; then
    echo "build_windows_mingw_envsetup.sh"
    source build_windows_mingw_envsetup.sh
fi

if [ -n "$1" ]; then
    SOURCE_CODE=$1
else
    SOURCE_CODE=${PREFIX}/../src/opencv
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone git://github.com/Itseez/opencv.git  ${SOURCE_CODE}"
    git clone git://github.com/Itseez/opencv.git  ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo ""

mkdir -p build_windows_mingw
cd build_windows_mingw
rm -fr *

echo "configure ..."
cmake .. \
        -G"Unix Makefiles" \
        -DCMAKE_INSTALL_PREFIX=${PREFIX} \
        -DBUILD_SHARED_LIBS=OFF \
        -DBUILD_DOCS=OFF \
        -DBUILD_opencv_apps=OFF \
        -DBUILD_EXAMPLES=OFF \
        -DBUILD_ANDROID_EXAMPLES=OFF \
        -DBUILD_TESTS=OFF \
        -DBUILD_FAT_JAVA_LIB=OFF \
        -DBUILD_JPEG=OFF \
        -DBUILD_OPENEXR=OFF \
        -DBUILD_PERF_TESTS=OFF \
        -DBUILD_PACKAGE=OFF \
        -DBUILD_PNG=OFF \
        -DBUILD_TBB=OFF \
        -DBUILD_TIFF=OFF \
        -DBUILD_WITH_DEBUG_INFO=OFF \
        -DWITH_OPENCL=OFF \
        -DBUILD_opencv_calib3d=OFF \
        -DBUILD_opencv_videostab=OFF \
        -DBUILD_opencv_ts=OFF \
        -DBUILD_opencv_java=OFF \
        -DWITH_WEBP=OFF \
        -DWITH_TIFF=OFF \
        -DWITH_JPEG=OFF \
        -DWITH_PNG=OFF \
        -DWITH_OPENEXR=OFF \
        -DWITH_WIN32UI=OFF \
        -DWITH_FFMPEG=OFF \
        -DWITH_1394=OFF \
        -DWITH_VTK=OFF \
        -DWITH_VFW=OFF \
        -DWITH_IPP=OFF \
        -DWITH_IPP_A=OFF \
        -DWITH_PVAPI=OFF \
        -DWITH_JASPER=OFF \
        -DWITH_OPENCLAMDFFT=OFF \
        -DWITH_OPENCLAMDBLAS=OFF \
        -DWITH_GIGEAPI=OFF \
        -DWITH_GSTREAMER=OFF \
        -DWITH_GTK=OFF \
        -DWITH_LIBV4L=OFF \
        -DBUILD_opencv_features2d=OFF \
        -DBUILD_opencv_flann=OFF \
        -DBUILD_opencv_highgui=OFF \
        -DWITH_V4L=OFF \
        -DBUILD_opencv_ml=OFF \
        -DBUILD_opencv_objdetect=OFF \
        -DBUILD_opencv_photo=OFF \
        -DBUILD_opencv_shape=OFF \
        -DBUILD_opencv_stitching=OFF \
        -DBUILD_opencv_superres=OFF \
        -DWITH_CUDA=OFF \
        -DWITH_CUFFT=OFF \
        -DWITH_DIRECTX=OFF \
        -DWITH_EIGEN:BOOL=OFF \
        -DWITH_DSHOW=ON \
        -DBUILD_opencv_video=ON \
        -DBUILD_opencv_videoio=ON

cmake --build . --target install --config Release

cp -fr ${PREFIX}/x64/mingw/staticlib/*.a ${PREFIX}/lib/.
rm -fr ${PREFIX}/x64

cd $CUR_DIR
