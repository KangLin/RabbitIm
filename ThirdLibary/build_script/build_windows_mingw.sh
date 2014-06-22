#参数：
#    $1:所有第三库依赖库存放目录

source build_windows_mingw_envsetup.sh

if [ -n "$1" ]; then
    source build_windows_mingw_speexdsp.sh $1/speexdsp
    source build_windows_mingw_speex.sh $1/speex
    source build_windows_mingw_libvpx.sh $1/libvpx
    source build_windows_mingw_opencv.sh $1/opencv
    source build_windows_mingw_x264.sh $1/x264
    source build_windows_mingw_ffmpeg.sh $1/ffmpeg
else
    source build_windows_mingw_speexdsp.sh
    source build_windows_mingw_speex.sh
    source build_windows_mingw_libvpx.sh
    source build_windows_mingw_opencv.sh
    source build_windows_mingw_x264.sh
    source build_windows_mingw_ffmpeg.sh
    source build_qxmpp.sh windows_mingw
fi
