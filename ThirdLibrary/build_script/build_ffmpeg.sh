#!/bin/bash
#bash用法：
#   在用一sh进程中执行脚本script.sh:
#   source script.sh
#   . script.sh
#   注意这种用法，script.sh开头一行不能包含 #!/bin/sh

#   新建一个sh进程执行脚本script.sh:
#   sh script.sh
#   ./script.sh
#   注意这种用法，script.sh开头一行必须包含 #!/bin/sh

#作者：康林
#参数:
#    $1:编译目标(android、windows_msvc、windows_mingw、unix)
#    $2:源码的位置 

#运行本脚本前,先运行 build_$1_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix）
#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_SOURCE_CODE    #源码目录
#   RABBITIM_BUILD_CROSS_PREFIX   #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

set -e
HELP_STRING="Usage $0 PLATFORM (android|windows_msvc|windows_mingw|unix) [SOURCE_CODE_ROOT_DIRECTORY]"

case $1 in
    android|windows_msvc|windows_mingw|unix)
    RABBITIM_BUILD_TARGERT=$1
    ;;
    *)
    echo "${HELP_STRING}"
    exit 1
    ;;
esac

if [ -z "${RABBITIM_BUILD_PREFIX}" ]; then
    echo ". `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh"
    . `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh
fi

if [ -n "$2" ]; then
    RABBITIM_BUILD_SOURCE_CODE=$2
else
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/ffmpeg
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    FFMPEG_VERSION=n3.0.2
    if [ "TRUE" = "${RABBITIM_USE_REPOSITORIES}" ]; then
        echo "git clone git://source.ffmpeg.org/ffmpeg.git ${RABBITIM_BUILD_SOURCE_CODE}"
        #git clone -q -b ${FFMPEG_VERSION} git://source.ffmpeg.org/ffmpeg.git ${RABBITIM_BUILD_SOURCE_CODE}
        git clone -q -b ${FFMPEG_VERSION} git://source.ffmpeg.org/ffmpeg.git ${RABBITIM_BUILD_SOURCE_CODE}
    else
        echo "wget -q https://github.com/FFmpeg/FFmpeg/archive/${FFMPEG_VERSION}.zip"
        mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        wget -q -c https://github.com/FFmpeg/FFmpeg/archive/${FFMPEG_VERSION}.zip
        echo "unzip ${FFMPEG_VERSION}.zip"
        unzip -q ${FFMPEG_VERSION}.zip
        mv FFmpeg-${FFMPEG_VERSION} ..
        rm -fr *
        cd ..
        rm -fr ${RABBITIM_BUILD_SOURCE_CODE}
        mv -f FFmpeg-${FFMPEG_VERSION} ${RABBITIM_BUILD_SOURCE_CODE}
    fi
fi

cd ${RABBITIM_BUILD_SOURCE_CODE}

echo "----------------------------------------------------------------------------"
echo "RABBITIM_BUILD_TARGERT:${RABBITIM_BUILD_TARGERT}"
echo "RABBITIM_BUILD_SOURCE_CODE:$RABBITIM_BUILD_SOURCE_CODE"
echo "CUR_DIR:`pwd`"
echo "RABBITIM_BUILD_PREFIX:$RABBITIM_BUILD_PREFIX"
echo "RABBITIM_BUILD_HOST:$RABBITIM_BUILD_HOST"
echo "RABBITIM_BUILD_CROSS_HOST:$RABBITIM_BUILD_CROSS_HOST"
echo "RABBITIM_BUILD_CROSS_PREFIX:$RABBITIM_BUILD_CROSS_PREFIX"
echo "RABBITIM_BUILD_CROSS_SYSROOT:$RABBITIM_BUILD_CROSS_SYSROOT"
echo "RABBITIM_BUILD_STATIC:$RABBITIM_BUILD_STATIC"
echo "PKG_CONFIG_PATH:${PKG_CONFIG_PATH}"
echo "PKG_CONFIG_LIBDIR:${PKG_CONFIG_LIBDIR}"
echo "----------------------------------------------------------------------------"

if [ "$RABBITIM_CLEAN" = "TRUE" ]; then
    if [ -d ".git" ]; then
        git clean -xdf
    elif [ -f "config.mak" ]; then
        make distclean
    fi
fi

echo "configure ..."
if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
    CONFIG_PARA="--enable-static --disable-shared"
    LDFLAGS="-static"
else
    CONFIG_PARA="--disable-static --enable-shared"
fi
THIRD_LIB="--enable-libx264"
case ${RABBITIM_BUILD_TARGERT} in
    android)
        CONFIG_PARA="--enable-cross-compile --target-os=linux --arch=arm --cpu=armv7-a --enable-neon"
        CONFIG_PARA="${CONFIG_PARA} --enable-static --disable-shared --disable-w32threads"
        CONFIG_PARA="${CONFIG_PARA} --cross-prefix=${RABBITIM_BUILD_CROSS_PREFIX}"
        CONFIG_PARA="${CONFIG_PARA} --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        #CONFIG_PARA="${CONFIG_PARA} --pkg-config="${PKG_CONFIG}"
        CONFIG_PARA="${CONFIG_PARA} --pkgconfigdir=${RABBITIM_BUILD_PREFIX}/lib/pkgconfig"
        CONFIG_PARA="${CONFIG_PARA} ${THIRD_LIB}"
        CFLAGS="-march=armv7-a -mfpu=neon"
        LDFLAGS="-lcpu-features --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}" 
        ;;
    unix)
        CONFIG_PARA="${CONFIG_PARA} ${THIRD_LIB}"
        ;;
    windows_msvc)
        CONFIG_PARA="${CONFIG_PARA} --target-os=win32 --arch=i686 --cpu=i686"
        CONFIG_PARA="${CONFIG_PARA} --enable-cross-compile --toolchain=msvc"
        ;;
    windows_mingw)
        CONFIG_PARA="${CONFIG_PARA} --enable-cross-compile --target-os=mingw32 --arch=i686 --cpu=i686"
        CONFIG_PARA="${CONFIG_PARA} ${THIRD_LIB}"
        case `uname -s` in
            MINGW*|MSYS*)
                ;;
            Linux*|Unix*|CYGWIN*|*)
                CONFIG_PARA="${CONFIG_PARA} --cross-prefix=${RABBITIM_BUILD_CROSS_PREFIX}"
                #CONFIG_PARA="${CONFIG_PARA} --pkg-config=${PKG_CONFIG}"
                CONFIG_PARA="${CONFIG_PARA} --pkgconfigdir=${RABBITIM_BUILD_PREFIX}/lib/pkgconfig"
                ;;
            *)
            echo "Don't support tagert:`uname -s`, please see build_ffmpeg.sh"
            exit 3
        esac
        ;;
    *)
        echo "${HELP_STRING}"
        cd $CUR_DIR
        exit 2
        ;;
esac

CONFIG_PARA="${CONFIG_PARA} --prefix=$RABBITIM_BUILD_PREFIX --enable-gpl --enable-pic --disable-doc --disable-htmlpages"
CONFIG_PARA="${CONFIG_PARA} --disable-manpages --disable-podpages --disable-txtpages  --disable-ffprobe"
#CONFIG_PARA="${CONFIG_PARA} --disable-ffserver --disable-ffplay --disable-programs"
CONFIG_PARA="${CONFIG_PARA} --enable-runtime-cpudetect"
CFLAGS="${CFLAGS} -I$RABBITIM_BUILD_PREFIX/include" 
LDFLAGS="${LDFLAGS} -L$RABBITIM_BUILD_PREFIX/lib" 


case ${RABBITIM_BUILD_TARGERT} in
    android)
        echo "./configure ${CONFIG_PARA} --extra-cflags=\"${CFLAGS}\" --extra-ldflags=\"${LDFLAGS}\"" --pkg-config="\"${PKG_CONFIG}\""
        ./configure ${CONFIG_PARA} --extra-cflags="${CFLAGS}" --extra-ldflags="${LDFLAGS}" --pkg-config="${PKG_CONFIG}"
        ;;
    windows_mingw)
        case `uname -s` in
            Linux*|Unix*|CYGWIN*|*)
            echo "./configure ${CONFIG_PARA} --extra-cflags=\"${CFLAGS}\" --extra-ldflags=\"${LDFLAGS}\"" --pkg-config="\"${PKG_CONFIG}\""
            ./configure ${CONFIG_PARA} --extra-cflags="${CFLAGS}" --extra-ldflags="${LDFLAGS}" --pkg-config="${PKG_CONFIG}"
            ;;
        esac
        ;;
    *)
        echo "./configure ${CONFIG_PARA} --extra-cflags=\"${CFLAGS}\" --extra-ldflags=\"${LDFLAGS}\""
        ./configure ${CONFIG_PARA} --extra-cflags="${CFLAGS}" --extra-ldflags="${LDFLAGS}"
        ;;
esac

echo "make install"
make ${RABBITIM_MAKE_JOB_PARA} && make install
if [ "${RABBITIM_BUILD_TARGERT}" = "windows_msvc" ]; then
    if [ "${RABBITIM_BUILD_STATIC}" = "static" ]; then
        cd ${RABBITIM_BUILD_PREFIX}/lib
        mv libavcodec.a avcodec.lib
        mv libavutil.a avutil.lib
        mv libpostproc.a postproc.lib
        mv libavfilter.a avfilter.lib
        mv libswresample.a swresample.lib
        mv libavformat.a avformat.lib
        mv libswscale.a swscale.lib
    else
        mv ${RABBITIM_BUILD_PREFIX}/bin/*.lib ${RABBITIM_BUILD_PREFIX}/lib/.
    fi
fi

if [ "${RABBITIM_BUILD_TARGERT}" = "windows_mingw" ]; then
    if [ "${RABBITIM_BUILD_STATIC}" != "static" ]; then
        mv ${RABBITIM_BUILD_PREFIX}/bin/*.lib ${RABBITIM_BUILD_PREFIX}/lib/.
    fi
fi

cd $CUR_DIR
