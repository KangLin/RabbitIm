#作者：康林
#参数:
#    $1:编译目标(android、windows_msvc、windows_mingw、unix、unix_mingw)
#    $2:源码的位置 

#运行本脚本前,先运行 build_$1_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix、unix_mingw）
#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_SOURCE_CODE    #源码目录
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

HELP_STRING="Usage $0 PLATFORM (android|windows_msvc|windows_mingw|unix|unix_mingw) SOURCE_CODE_ROOT"

case $1 in
    android|windows_msvc|windows_mingw|unix|unix_mingw)
    RABBITIM_BUILD_TARGERT=$1
    ;;
    *)
    echo "${HELP_STRING}"
    return 1
    ;;
esac

#运行本脚本前,先运行 build_windows_mingw_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   PREFIX=`pwd`/../windows  #修改这里为安装前缀
#   QMAKE=  #设置用于相应平台编译的 QMAKE
#   JOM=    #QT 自带的类似 make 的工具
if [ -z "${PREFIX}" -o -z "${QMAKE}" -o -z "${JOM}" ]; then
    echo "build_${RABBITIM_BUILD_TARGERT}_envsetup.sh"
    source build_${RABBITIM_BUILD_TARGERT}_envsetup.sh
fi

if [ -n "$2" ]; then
    RABBITIM_BUILD_SOURCE_CODE=$2
else
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/qxmpp
fi

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    echo "git clone https://github.com/qxmpp-project/qxmpp.git ${RABBITIM_BUILD_SOURCE_CODE}"
    git clone https://github.com/qxmpp-project/qxmpp.git ${RABBITIM_BUILD_SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${RABBITIM_BUILD_SOURCE_CODE}

mkdir -p build_${RABBITIM_BUILD_TARGERT}
cd build_${RABBITIM_BUILD_TARGERT}
rm -fr *

echo ""
echo "RABBITIM_BUILD_TARGERT:${RABBITIM_BUILD_TARGERT}"
echo "RABBITIM_BUILD_SOURCE_CODE:$RABBITIM_BUILD_SOURCE_CODE"
echo "CUR_DIR:`pwd`"
echo "RABBITIM_BUILD_PREFIX:$RABBITIM_BUILD_PREFIX"
echo "RABBITIM_BUILD_CROSS_PREFIX:$RABBITIM_BUILD_CROSS_PREFIX"
echo "RABBITIM_BUILD_CROSS_SYSROOT:$RABBITIM_BUILD_CROSS_SYSROOT"
echo ""

MAKE=make
case $RABBITIM_BUILD_TARGERT in
    android)
    PARA=" -r -spec android-g++"
    MAKE_PARA=" INSTALL_ROOT=\"${RABBITIM_BUILD_PREFIX}\""
    #MAKE=mingw32-make #mingw 中编译
    #MAKE="$ANDROID_NDK/prebuilt/${RABBITIM_BUILD_HOST}/bin/make"  #在windows下编译
    #make -f Makefile install INSTALL_ROOT="${RABBITIM_BUILD_PREFIX}"      #在linux下编译
     ;;
    unix)
    ;;
    windows_msvc)
    MAKE=${JOM}
    ;;
    windows_mingw)
    ;;
    *)
    echo "Usage $0 PLATFORM(android/windows_msvc/windows_mingw/unix) SOURCE_CODE_ROOT"
    return 1
    ;;
esac

$QMAKE -o Makefile \
       PREFIX=${RABBITIM_BUILD_PREFIX} \
       INCLUDEPATH+=${RABBITIM_BUILD_PREFIX}/include \
       LIBS+=-L${RABBITIM_BUILD_PREFIX}/lib \
       QXMPP_USE_VPX=1 \
       ${PARA} \
       .. #"CONFIG+=debug" #QXMPP_LIBRARY_TYPE=staticlib #静态库

${MAKE} -f Makefile install
if [ "$RABBITIM_BUILD_TARGERT" == "android" ]; then
    ${MAKE} -f Makefile install ${MAKE_PARA}
    cp -fr ${RABBITIM_BUILD_PREFIX}/libs/armeabi-v7a/* ${RABBITIM_BUILD_PREFIX}/lib
fi

cd $CUR_DIR
