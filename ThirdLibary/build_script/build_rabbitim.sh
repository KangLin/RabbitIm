#!/bin/sh

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
HELP_STRING="Usage $0 PLATFORM (android|windows_msvc|windows_mingw|unix) [SOURCE_CODE_ROOT_DIRECTORY] [qmake]"

case $1 in
    android|windows_msvc|windows_mingw|unix)
    RABBITIM_BUILD_TARGERT=$1
    ;;
    *)
    echo "${HELP_STRING}"
    exit 1
    ;;
esac

#运行本脚本前,先运行 build_${RABBITIM_BUILD_TARGERT}_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_PREFIX= #修改这里为安装前缀
#   QMAKE=  #设置用于相应平台编译的 QMAKE
#   JOM=    #QT 自带的类似 make 的工具
if [ -z "${PREFIX}" -o -z "${QMAKE}" -o -z "${JOM}" ]; then
    echo ". `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh"
    . `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh
fi

if [ -n "$2" ]; then
    RABBITIM_BUILD_SOURCE_CODE=$2
else
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../..
fi

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    echo "git clone git@github.com:KangLin/rabbitim.git"
    git clone git@github.com:KangLin/rabbitim.git ${RABBITIM_BUILD_SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${RABBITIM_BUILD_SOURCE_CODE}

echo ""
echo "RABBITIM_BUILD_TARGERT:${RABBITIM_BUILD_TARGERT}"
echo "RABBITIM_BUILD_SOURCE_CODE:$RABBITIM_BUILD_SOURCE_CODE"

echo "RABBITIM_BUILD_PREFIX:$RABBITIM_BUILD_PREFIX"
echo "RABBITIM_BUILD_CROSS_PREFIX:$RABBITIM_BUILD_CROSS_PREFIX"
echo "RABBITIM_BUILD_CROSS_SYSROOT:$RABBITIM_BUILD_CROSS_SYSROOT"
echo "RABBITIM_BUILD_CROSS_HOST:$RABBITIM_BUILD_CROSS_HOST"
echo "RABBITIM_BUILD_HOST:$RABBITIM_BUILD_HOST"

mkdir -p build_${RABBITIM_BUILD_TARGERT}
cd build_${RABBITIM_BUILD_TARGERT}
rm -fr *

echo "CUR_DIR:`pwd`"
echo ""

if [ "$3" = "qmake" ]; then
    MAKE="make ${RABBITIM_MAKE_JOB_PARA}"
    case $1 in
        android)
            PARA="-r -spec android-g++ " #RABBITIM_USER_OPENCV=1
            if [ -n "$RABBITIM_CMAKE_MAKE_PROGRAM" ]; then
                MAKE="$RABBITIM_CMAKE_MAKE_PROGRAM" 
            fi
            ;;
        unix)
            PARA="-r -spec linux-g++ "
            ;;
        windows_msvc)
            PARA="-r -spec win32-msvc2013"
            MAKE=${JOM}
            ;;
        windows_mingw)
            PARA="-spec win32-g++"
            ;;
        *)
            echo "${HELP_STRING}"
            exit 1
            ;;
    esac
    $QMAKE .. $PARA PREFIX=`pwd`/install \
           INCLUDEPATH+=${RABBITIM_BUILD_PREFIX}/include \
           LIBS+=-L${RABBITIM_BUILD_PREFIX}/lib \
           QXMPP_USE_VPX=1 \
           RABBITIM_USER_FFMPEG=1 \
           RABBITIM_USER_LIBCURL=1 \
           RABBITIM_USER_OPENSSL=1 
    if [ "$1" == "android" ]; then
        $MAKE -f Makefile install INSTALL_ROOT="`pwd`/android-build"
        ${QT_BIN}/androiddeployqt --input "`pwd`/android-libRabbitIm.so-deployment-settings.json" --output "`pwd`/android-build" --verbose
    else
        $MAKE -f Makefile install -j 2
    fi
else
    GENERATORS="Unix Makefiles" 
    CMAKE_PARA="--target package"
    PARA="-DCMAKE_BUILD_TYPE=Release -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5"
    MAKE_PARA="-- ${RABBITIM_MAKE_JOB_PARA}  VERBOSE=1"
    case $1 in
        android)
            PARA="${PARA} -DCMAKE_TOOLCHAIN_FILE=${RABBITIM_BUILD_SOURCE_CODE}/cmake/platforms/toolchain-android.cmake -DOPTION_RABBITIM_USER_OPENCV=ON"
            PARA="${PARA} -DANT=${ANT} "
            CMAKE_PARA=""
            ;;
        unix)
            PARA="-DCMAKE_INSTALL_PREFIX=/usr/local/RabbitIm"
            ;;
        windows_msvc)
            GENERATORS="NMake Makefiles"
            PARA="${PARA} -DOPTION_RABBITIM_USER_LIBCURL=OFF -DOPTION_RABBITIM_USER_OPENSSL=OFF"
            MAKE_PARA=""
            ;;
        windows_mingw)
            PARA="${PARA} -DCMAKE_TOOLCHAIN_FILE=${RABBITIM_BUILD_SOURCE_CODE}/cmake/platforms/toolchain-mingw.cmake"
            PARA="${PARA} -DOPTION_RABBITIM_USER_LIBCURL=ON -DOPTION_RABBITIM_USER_OPENSSL=ON"
            ;;
        *)
            echo "${HELP_STRING}"
            cd $CUR_DIR
            exit 1
            ;;
    esac
    echo "cmake .. -G\"${GENERATORS}\" $PARA"
    cmake .. -G"${GENERATORS}" $PARA
    cmake --build .  --config Release ${CMAKE_PARA} ${MAKE_PARA}
fi

cd $CUR_DIR
