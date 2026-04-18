#!/bin/bash
# Author: Kang Lin <kl222@126.com>

# See: - https://docs.appimage.org/packaging-guide/from-source/native-binaries.html#examples
#      - https://luyuhuang.tech/2024/04/19/appimage.html

#See: https://blog.csdn.net/alwaysbefine/article/details/114187380
#set -x
set -e
#set -v

if [ -z "$BUILD_VERBOSE" ]; then
    BUILD_VERBOSE=OFF
fi

source $(dirname $(readlink -f $0))/common.sh

usage_long() {
    echo "$0 [--install=<install directory>] [ [-h|--help] [-v|--verbose[=0|1]] --source=<source directory>] [--tools=<tools directory>] [--build=<build directory>]"
    echo "  --help|-h: Show help"
    echo "  -v|--verbose: Show build verbose"
    echo "Directory:"
    echo "  --install: Set install directory"
    echo "  --source: Set source directory"
    echo "  --tools: Set tools directory"
    echo "  --build: set build directory"
    exit
}

# [如何使用getopt和getopts命令解析命令行选项和参数](https://zhuanlan.zhihu.com/p/673908518)
# [【Linux】Shell命令 getopts/getopt用法详解](https://blog.csdn.net/arpospf/article/details/103381621)
if command -V getopt >/dev/null; then
    echo "getopt is exits"
    #echo "original parameters=[$@]"
    # -o 或 --options 选项后面是可接受的短选项，如 ab:c:: ，表示可接受的短选项为 -a -b -c ，
    # 其中 -a 选项不接参数，-b 选项后必须接参数，-c 选项的参数为可选的
    # 后面没有冒号表示没有参数。后跟有一个冒号表示有参数。跟两个冒号表示有可选参数。
    # -l 或 --long 选项后面是可接受的长选项，用逗号分开，冒号的意义同短选项。
    # -n 选项后接选项解析错误时提示的脚本名字
    OPTS=help,verbose::,install:,source:,tools:,build:
    ARGS=`getopt -o h,v:: -l $OPTS -n $(basename $0) -- "$@"`
    if [ $? != 0 ]; then
        echo_error "exec getopt fail: $?"
        exit 1
    fi
    #echo "ARGS=[$ARGS]"
    #将规范化后的命令行参数分配至位置参数（$1,$2,......)
    eval set -- "${ARGS}"
    #echo "formatted parameters=[$@]"

    while [ $1 ]
    do
        #echo "\$1: $1"
        #echo "\$2: $2"
        case $1 in
        -v |--verbose)
            case $2 in
                "")
                    BUILD_VERBOSE=ON;;
                *)
                    BUILD_VERBOSE=$2;;
            esac
            shift 2
            ;;
        --install)
            INSTALL_DIR=$2
            shift 2
            ;;
        --source)
            SOURCE_DIR=$2
            shift 2
            ;;
        --tools)
            TOOLS_DIR=$2
            shift 2
            ;;
        --build)
            BUILD_DIR=$2
            shift 2
            ;;
        --) # 当解析到“选项和参数“与“non-option parameters“的分隔符时终止
            shift
            break
            ;;
        -h | -help)
            usage_long
            shift
            ;;
        *)
            usage_long
            break
            ;;
        esac
    done
fi

## building in temporary directory to keep system clean
## use RAM disk if possible (as in: not building on CI system like Travis, and RAM disk is available)
#if [ "$CI" == "" ] && [ -d /dev/shm ]; then
#    TEMP_BASE=/dev/shm
#else
#    TEMP_BASE=/tmp
#fi
#BUILD_APPIMAGE_DIR=$(mktemp -d -p "$TEMP_BASE" RabbitIm-appimage-build-XXXXXX)
## make sure to clean up build dir, even if errors occur

# store repo root as variable
REPO_ROOT=$(safe_readlink $(dirname $(dirname $(safe_readlink $0))))
OLD_CWD=$(safe_readlink .)

pushd "$REPO_ROOT"

if [ -z "$BUILD_APPIMAGE_DIR" ]; then
    if [ -z "$BUILD_DIR" ]; then
        BUILD_APPIMAGE_DIR=build_appimage
    else
        BUILD_APPIMAGE_DIR=$BUILD_DIR/build_appimage
    fi
fi
BUILD_APPIMAGE_DIR=$(safe_readlink ${BUILD_APPIMAGE_DIR})
mkdir -p $BUILD_APPIMAGE_DIR
pushd "$BUILD_APPIMAGE_DIR"

if [ -z "$TOOLS_DIR" ]; then
    TOOLS_DIR=Tools
fi
if [ -z "$SOURCE_DIR" ]; then
    SOURCE_DIR=Source
fi
if [ -z "$INSTALL_DIR" ]; then
    INSTALL_DIR=Install
fi

TOOLS_DIR=$(safe_readlink ${TOOLS_DIR})
mkdir -p $TOOLS_DIR
SOURCE_DIR=$(safe_readlink ${SOURCE_DIR})
mkdir -p $SOURCE_DIR
INSTALL_DIR=$(safe_readlink ${INSTALL_DIR})
mkdir -p $INSTALL_DIR

echo "Repo folder: $REPO_ROOT"
echo "Old folder: $OLD_CWD"
echo "Current folder: `pwd`"
echo "BUILD_APPIMAGE_DIR: $BUILD_APPIMAGE_DIR"
echo "TOOLS_DIR: $TOOLS_DIR"
echo "SOURCE_DIR: $SOURCE_DIR"
echo "INSTALL_DIR: $INSTALL_DIR"

cleanup () {
    if [ -d "${BUILD_APPIMAGE_DIR}" ]; then
        rm -rf "${BUILD_APPIMAGE_DIR}"
    fi
}
if [ "$CI" != "" ]; then
    trap cleanup EXIT
fi

echo_status "Download linuxdeploy ......"
pushd "${TOOLS_DIR}"
if [ ! -f linuxdeploy-`uname -m`.AppImage ]; then
    wget https://github.com/linuxdeploy/linuxdeploy/releases/download/1-alpha-20251107-1/linuxdeploy-`uname -m`.AppImage
    chmod u+x linuxdeploy-`uname -m`.AppImage
fi
if [ ! -f linuxdeploy-plugin-qt-`uname -m`.AppImage ]; then
    wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/1-alpha-20250213-1/linuxdeploy-plugin-qt-`uname -m`.AppImage
    chmod u+x linuxdeploy-plugin-qt-`uname -m`.AppImage
fi
popd

echo_status "Compile RabbitIm ......"
if [ "${BUILD_VERBOSE}" = "ON" -a -n "$QMAKE" ]; then
    echo "QT_ROOT: $QT_ROOT"
    echo "Qt6_DIR: $Qt6_DIR"
    echo "QMAKE: $QMAKE"
    echo "LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
    echo "PATH: $PATH"
    $QMAKE --version
fi
INSTALL_APP_DIR=AppDir/usr
if [ -n "${INSTALL_DIR}" ]; then
    export CMAKE_PREFIX_PATH=${INSTALL_DIR}:${CMAKE_PREFIX_PATH}
fi
cmake "$REPO_ROOT" \
  -DCMAKE_INSTALL_PREFIX=/usr \
  -DCMAKE_VERBOSE_MAKEFILE=${BUILD_VERBOSE} \
  -DCMARK_SHARED=OFF \
  -DCMARK_TESTS=OFF \
  -DCMARK_STATIC=ON \
  -DWITH_CMARK=OFF \
  -DWITH_CMARK_GFM=ON \
  -DENABLE_UPDATE_TRANSLATIONS=ON \
  -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --parallel $(nproc)
cmake --install . --config Release --strip --component DependLibraries --prefix ${INSTALL_APP_DIR}
cmake --install . --config Release --strip --component Runtime --prefix ${INSTALL_APP_DIR}
cmake --install . --config Release --strip --component Plugin --prefix ${INSTALL_APP_DIR}
cmake --install . --config Release --strip --component Application --prefix ${INSTALL_APP_DIR}


echo_status "Build AppImage ......"
# See: https://github.com/linuxdeploy/linuxdeploy-plugin-qt
#export QMAKE=$QT_ROOT/bin/qmake6
#export PATH=$QT_ROOT/libexec:$PATH
export EXTRA_PLATFORM_PLUGINS="libqxcb.so;libqvnc.so"
export DEPLOY_PLATFORM_THEMES=true
# Icons from theme are not displayed in QtWidgets Application: https://github.com/linuxdeploy/linuxdeploy-plugin-qt/issues/17
# qtmodules: https://doc.qt.io/archives/qt-6.7/qtmodules.html
export EXTRA_QT_MODULES="svg;sql;multimedia;statemachine"
export PATH=$PATH:${TOOLS_DIR}

if [ "${BUILD_VERBOSE}" = "ON" -a -n "$QMAKE" ]; then
    echo "QT_ROOT: $QT_ROOT"
    echo "Qt6_DIR: $Qt6_DIR"
    echo "QMAKE: $QMAKE"
    echo "EXTRA_PLATFORM_PLUGINS: $EXTRA_PLATFORM_PLUGINS"
    echo "EXTRA_QT_MODULES: $EXTRA_QT_MODULES"
    echo "LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
    echo "QT_PLUGIN_PATH: $QT_PLUGIN_PATH"
    echo "PATH: $PATH"
    $QMAKE --version
fi

if [ -n "$QMAKE" ]; then
    if command -v qmake >/dev/null 2>&1; then
        command -v qmake
    else
        echo_error "Please set 'QMAKE'"
    fi
fi

case "$DISTRO" in
ubuntu|debian)
    DEPLOY_PARA=
    ;;
fedora)
    export NO_STRIP=true
    ;;
esac

# [linuxdeploy user guide](https://docs.appimage.org/packaging-guide/from-source/linuxdeploy-user-guide.html)
${TOOLS_DIR}/linuxdeploy-`uname -m`.AppImage --appdir=AppDir ${DEPLOY_PARA} \
    --plugin qt \
    --output appimage \
    --deploy-deps-only=${INSTALL_APP_DIR}/plugins

chmod a+x RabbitIm-`uname -m`.AppImage

cp RabbitIm-`uname -m`.AppImage $REPO_ROOT/RabbitIm_`uname -m`.AppImage
echo_status "Generated AppImage: $REPO_ROOT/RabbitIm_`uname -m`.AppImage"

popd
popd
