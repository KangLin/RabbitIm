#参数:
#    $1:编译平台(android、windows_msvc、windows_mingw、unix)
#    $2:源码的位置(省略时，会自动下载源码)

HELP_STRING="Usage $0 PLATFORM (android|windows_msvc|windows_mingw|unix) SOURCE_CODE_ROOT [cmake]"
case $1 in
    android | windows_msvc | windows_mingw | unix)
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
if [ -z "${PREFIX}" -o -z "${JOM}" ]; then
    echo "source build_$1_envsetup.sh"
    source build_$1_envsetup.sh
fi

if [ -n "$2" ]; then
    SOURCE_CODE=$2
else
    SOURCE_CODE=${PREFIX}/../..
fi

#下载源码:
if [ ! -d ${SOURCE_CODE} ]; then
    echo "git clone https://code.csdn.net/kl222/rabbitim.git"
    git clone https://code.csdn.net/kl222/rabbitim.git ${SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${SOURCE_CODE}

echo ""
echo "PATH:$PATH"
echo "SOURCE_CODE:$SOURCE_CODE"
echo "CUR_DIR:$CUR_DIR"
echo "PREFIX:$PREFIX"
echo "QMAKE:$QMAKE"
echo "ANT:${ANT}"
echo "\$1:$1"
echo "\$2:$2"
echo ""

mkdir -p ${SOURCE_CODE}/build_$1
cd ${SOURCE_CODE}/build_$1
echo "Current dir:`pwd`"
rm -fr *

if [ "$3" == "cmake" ]; then
    GENERATORS="Unix Makefiles" 
    PARA="-DCMAKE_BUILD_TYPE=Release -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5"
    case $1 in
        android)
            PARA="${PARA} -DCMAKE_TOOLCHAIN_FILE=${PREFIX}/../../platforms/android/android.toolchain.cmake -DOPTION_RABBITIM_USER_OPENCV=ON"
            PARA="${PARA} -DANT=${ANT} "
            ;;
        unix)
            ;;
        windows_msvc)
            GENERATORS="NMake Makefiles"
            ;;
        windows_mingw)
            ;;
        *)
            echo "${HELP_STRING}"
            return 1
            ;;
    esac
    echo "cmake .. -G\"${GENERATORS}\" $PARA"
    cmake .. -G"${GENERATORS}" $PARA
    cmake --build . --config Release --target package 
else
    case $1 in
        android)
            PARA="-r -spec android-g++ RABBITIM_USER_OPENCV=1"
            MAKE="$ANDROID_NDK/prebuilt/${HOST}/bin/make"
            ;;
        unix)
            PARA="-r -spec linux-g++ "
            MAKE="make"
            ;;
        windows_msvc)
            PARA="-r -spec win32-msvc2013"
            MAKE=${JOM}
            ;;
        windows_mingw)
            PARA="-spec win32-g++"
            MAKE="make"
            ;;
        *)
            echo "${HELP_STRING}"
            return 1
            ;;
    esac
    $QMAKE .. $PARA PREFIX=`pwd`/install \
           INCLUDEPATH+=${PREFIX}/include \
           LIBS+=-L${PREFIX}/lib \
           QXMPP_USE_VPX=1 \
           RABBITIM_USER_FFMPEG=1 \
           RABBITIM_USER_LIBCURL=1 \
           RABBITIM_USER_OPENSSL=1 
    if [ "$1" == "android" ]; then
        $MAKE -f Makefile install INSTALL_ROOT="`pwd`/android-build"
        ${QT_BIN}/androiddeployqt --input "`pwd`/android-libRabbitIm.so-deployment-settings.json" --output "`pwd`/android-build" --verbose
    else
        $MAKE -f Makefile install
    fi
fi

cd $CUR_DIR
