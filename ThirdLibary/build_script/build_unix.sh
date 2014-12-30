
#参数：
#    $1:所有第三库依赖库存放目录

source build_unix_envsetup.sh

if [ -n "$1" ]; then
    #source build_unix_speexdsp.sh $1/speexdsp
    #source build_unix_speex.sh $1/speex
    source build_unix_x264.sh $1/x264
    source build_unix_ffmpeg.sh $1/ffmpeg
    #source build_unix_opencv.sh $1/opencv
    source build_qxmpp.sh unix $1/qxmpp
else
    source build_unix_speexdsp.sh
    source build_unix_speex.sh
    source build_unix_x264.sh
    source build_unix_ffmpeg.sh
    source build_unix_opencv.sh
    source build_qxmpp.sh unix
fi
