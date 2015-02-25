#参数:
#    $1:编译平台(android、windows_msvc、windows_mingw、unix)
#    $2:源码的位置(省略时，会自动下载源码)

case $1 in
    android | windows_msvc | windows_mingw | unix)
    ;;
    *)
    echo "Usage $0 PLATFORM(android/windows_msvc/windows_mingw/unix) SOURCE_CODE_ROOT"
    exit 1
    ;;
esac

#运行本脚本前,先运行 build_windows_mingw_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   PREFIX=`pwd`/../windows  #修改这里为安装前缀
#   QMAKE=  #设置用于相应平台编译的 QMAKE
#   JOM=    #QT 自带的类似 make 的工具
if [ -z "${PREFIX}" -o -z "${QMAKE}" -o -z "${JOM}" ]; then
    echo "source build_$1_envsetup.sh"
    source build_$1_envsetup.sh
fi

if [ -n "$2" ]; then
    SOURCE_CODE=$2
else
    SOURCE_CODE=${PREFIX}/../src/qxmpp
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone https://github.com/qxmpp-project/qxmpp.git ${SOURCE_CODE}"
    git clone https://github.com/qxmpp-project/qxmpp.git ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo "QMAKE:$QMAKE"
echo "JOM:$JOM"
echo "\$1:$1"
echo "\$2:$2"
echo ""

mkdir -p ${SOURCE_CODE}/build_$1
cd ${SOURCE_CODE}/build_$1
echo "Current dir:`pwd`"
rm -fr *

$QMAKE -o Makefile \
       PREFIX=${PREFIX} \
       INCLUDEPATH+=${PREFIX}/include \
       LIBS+=-L${PREFIX}/lib \
       QXMPP_USE_VPX=1 \
       .. #"CONFIG+=debug" #QXMPP_LIBRARY_TYPE=staticlib #静态库

case $1 in
    android)
    "$ANDROID_NDK/prebuilt/${HOST}/bin/make" -f Makefile install
    ;;
    unix)
    make install -f Makefile -j 2
    ;;
    windows_msvc)
    ${JOM} -f Makefile install
    ;;
    windows_mingw)
    make install -f Makefile
    ;;
    *)
    echo "Usage $0 PLATFORM(android/windows_msvc/windows_mingw/unix) SOURCE_CODE_ROOT"
    return 1
    ;;
esac

cd $CUR_DIR
