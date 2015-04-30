#!/bin/sh

#作者：康林
#参数:
#    $1:编译目标(android、windows_msvc、windows_mingw、unix)
#    $2:源码的位置 

#运行本脚本前,先运行 build_$1_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix)
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

#运行本脚本前,先运行 build_${RABBITIM_BUILD_TARGERT}_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   PREFIX= #修改这里为安装前缀
#   QMAKE=  #设置用于相应平台编译的 QMAKE
#   JOM=    #QT 自带的类似 make 的工具
if [ -z "${PREFIX}" -o -z "${QMAKE}" -o -z "${JOM}" ]; then
    echo ". `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh"
    . `pwd`/build_envsetup_${RABBITIM_BUILD_TARGERT}.sh
fi

if [ -n "$2" ]; then
    RABBITIM_BUILD_SOURCE_CODE=$2
else
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/qxmpp
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    if [ "TRUE" = "$RABBITIM_USE_REPOSITORIES" ]; then
        echo "git clone https://github.com/qxmpp-project/qxmpp.git ${RABBITIM_BUILD_SOURCE_CODE}"
        git clone https://github.com/KangLin/qxmpp.git ${RABBITIM_BUILD_SOURCE_CODE}
    else
        mkdir -p ${RABBITIM_BUILD_SOURCE_CODE}
        cd ${RABBITIM_BUILD_SOURCE_CODE}
        wget https://github.com/KangLin/qxmpp/archive/master.zip
        unzip master.zip
        mv qxmpp-master ..
        rm -fr *
        cd ..
        mv -f qxmpp-master ${RABBITIM_BUILD_SOURCE_CODE}
    fi
fi

cd ${RABBITIM_BUILD_SOURCE_CODE}

mkdir -p build_${RABBITIM_BUILD_TARGERT}
cd build_${RABBITIM_BUILD_TARGERT}
if [ -n "$RABBITIM_CLEAN" ]; then
    rm -fr *
fi

echo ""
echo "RABBITIM_BUILD_TARGERT:${RABBITIM_BUILD_TARGERT}"
echo "RABBITIM_BUILD_SOURCE_CODE:$RABBITIM_BUILD_SOURCE_CODE"
echo "CUR_DIR:`pwd`"
echo "RABBITIM_BUILD_PREFIX:$RABBITIM_BUILD_PREFIX"
echo "RABBITIM_BUILD_HOST:$RABBITIM_BUILD_HOST"
echo "RABBITIM_BUILD_CROSS_HOST:$RABBITIM_BUILD_CROSS_HOST"
echo "RABBITIM_BUILD_CROSS_PREFIX:$RABBITIM_BUILD_CROSS_PREFIX"
echo "RABBITIM_BUILD_CROSS_SYSROOT:$RABBITIM_BUILD_CROSS_SYSROOT"
echo "RABBITIM_BUILD_STATIC:$RABBITIM_BUILD_STATIC"
echo ""

MAKE="make ${RABBITIM_MAKE_JOB_PARA}"
case $RABBITIM_BUILD_TARGERT in
    android)
        PARA=" -r -spec android-g++"
        MAKE_PARA=" INSTALL_ROOT=\"${RABBITIM_BUILD_PREFIX}\""
        #MAKE=mingw32-make #mingw 中编译
        #MAKE="$ANDROID_NDK/prebuilt/${RABBITIM_BUILD_HOST}/bin/make"     #在windows下编译
        #make -f Makefile install INSTALL_ROOT="${RABBITIM_BUILD_PREFIX}" #在linux下编译
         ;;
    unix)
        ;;
    windows_msvc)
        MAKE=nmake
        ;;
    windows_mingw)
        #PARA="-r -spec win32-g++" # CROSS_COMPILE=${RABBITIM_BUILD_CROSS_PREFIX}"
        ;;
    *)
        echo "Usage $0 PLATFORM(android/windows_msvc/windows_mingw/unix) SOURCE_CODE_ROOT"
        cd $CUR_DIR
        exit 2
        ;;
esac

if [ "$RABBITIM_BUILD_STATIC" = "static" ]; then
    PARA="${PARA} QXMPP_LIBRARY_TYPE=staticlib" #静态库
fi

$QMAKE -o Makefile \
       PREFIX=${RABBITIM_BUILD_PREFIX} \
       INCLUDEPATH+=${RABBITIM_BUILD_PREFIX}/include \
       LIBS+=-L${RABBITIM_BUILD_PREFIX}/lib \
       QXMPP_USE_VPX=1 \
       ${PARA} CONFIG+=release \
       ..

${MAKE} -f Makefile 
case $RABBITIM_BUILD_TARGERT in
    android)
        make -f Makefile install
        ${MAKE} -f Makefile install ${MAKE_PARA}
        cp -fr ${RABBITIM_BUILD_PREFIX}/libs/armeabi-v7a/* ${RABBITIM_BUILD_PREFIX}/lib
    ;;
    windows_mingw|windows_msvc)
        ${MAKE} install
        QXMPP_DLL="${RABBITIM_BUILD_PREFIX}/lib/qxmpp0.dll"
        if [ -f "${QXMPP_DLL}" ]; then
            mv ${QXMPP_DLL} ${RABBITIM_BUILD_PREFIX}/bin/.
        fi
    ;;
    *)
        ${MAKE} install
        if [ ! -f "${RABBITIM_BUILD_PREFIX}/lib/pkgconfig/qxmpp.pc" -a -d `pwd`/src/pkgconfig ]; then
            cp -fr `pwd`/src/pkgconfig/* ${RABBITIM_BUILD_PREFIX}/lib/pkgconfig/.
        fi
    ;;
esac

cd $CUR_DIR
