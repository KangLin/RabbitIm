#!/bin/sh
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
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
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
    FFMPEG_VERSION=2.6.1
    if [ -n "$RABBITIM_USE_REPOSITORIES" ]; then
        echo "git clone git://source.ffmpeg.org/ffmpeg.git ${RABBITIM_BUILD_SOURCE_CODE}"
        #git clone --branch=n${FFMPEG_VERSION} git://source.ffmpeg.org/ffmpeg.git ${RABBITIM_BUILD_SOURCE_CODE}
        git clone git://source.ffmpeg.org/ffmpeg.git ${RABBITIM_BUILD_SOURCE_CODE}
    else
        echo "wget http://ffmpeg.org/releases/ffmpeg-${FFMPEG_FILE}.tar.gz"
        mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        wget http://ffmpeg.org/releases/ffmpeg-${FFMPEG_FILE}.tar.gz
        tar xzvf ffmpeg-${FFMPEG_FILE}.tar.gz
        RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_SOURCE_CODE}/ffmpeg-${FFMPEG_VERSION}
    fi
fi

cd ${RABBITIM_BUILD_SOURCE_CODE}

echo ""
echo "RABBITIM_BUILD_TARGERT:${RABBITIM_BUILD_TARGERT}"
echo "RABBITIM_BUILD_SOURCE_CODE:$RABBITIM_BUILD_SOURCE_CODE"
echo "CUR_DIR:`pwd`"
echo "RABBITIM_BUILD_PREFIX:$RABBITIM_BUILD_PREFIX"
echo "RABBITIM_BUILD_HOST:$RABBITIM_BUILD_HOST"
echo "RABBITIM_BUILD_CROSS_HOST:$RABBITIM_BUILD_CROSS_HOST"
echo "RABBITIM_BUILD_CROSS_PREFIX:$RABBITIM_BUILD_CROSS_PREFIX"
echo "RABBITIM_BUILD_CROSS_SYSROOT:$RABBITIM_BUILD_CROSS_SYSROOT"
echo ""

if [ -d ".git" ]; then
    git clean -xdf
else
    make distclean
fi

echo "configure ..."
CONFIG_PARA="--disable-static --enable-shared"
THIRD_LIB="--enable-libx264"
case ${RABBITIM_BUILD_TARGERT} in
    android)
        CONFIG_PARA="--enable-cross-compile --target-os=linux --arch=arm --cpu=armv7-a --enable-neon"
        CONFIG_PARA="${CONFIG_PARA} --enable-static --disable-shared --disable-w32threads"
        CONFIG_PARA="${CONFIG_PARA} --cross-prefix=${RABBITIM_BUILD_CROSS_PREFIX}"
        CONFIG_PARA="${CONFIG_PARA} --sysroot=${RABBITIM_BUILD_CROSS_SYSROOT}"
        CONFIG_PARA="${CONFIG_PARA} ${THIRD_LIB}"
        CFLAGS="-march=armv7-a -mfpu=neon"
        LDFLAGS="-lcpu-features" 
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
CONFIG_PARA="${CONFIG_PARA} --disable-manpages --disable-podpages --disable-txtpages --disable-programs --disable-ffprobe"
CONFIG_PARA="${CONFIG_PARA} --disable-ffserver --disable-ffplay --disable-avdevice"
CONFIG_PARA="${CONFIG_PARA} --enable-runtime-cpudetect"
CFLAGS="${CFLAGS} -I$RABBITIM_BUILD_PREFIX/include" 
LDFLAGS="${LDFLAGS} -L$RABBITIM_BUILD_PREFIX/lib" 

echo "./configure ${CONFIG_PARA}  --extra-cflags=\"${CFLAGS}\" --extra-ldflags=\"${LDFLAGS}\""
./configure ${CONFIG_PARA} --extra-cflags="${CFLAGS}" --extra-ldflags="${LDFLAGS}"

echo "make install"
make ${RABBITIM_MAKE_JOB_PARA} && make install

cd $CUR_DIR
