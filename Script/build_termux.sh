#!/bin/bash
# Build the project in termux
# Author: Kang Lin <kl222@126.com>

set -e
#set -x

# 安全的 readlink 函数，兼容各种系统
safe_readlink() {
    local path="$1"
    if [ -L "$path" ]; then
        if command -v readlink >/dev/null 2>&1; then
            if readlink -f "$path" >/dev/null 2>&1; then
                readlink -f "$path"
            else
                readlink "$path"
            fi
        else
            ls -l "$path" | awk '{print $NF}'
        fi
    elif [ -e "$path" ]; then
        if command -v realpath >/dev/null 2>&1; then
            realpath "$path"
        else
            echo "$(cd "$(dirname "$path")" && pwd)/$(basename "$path")"
        fi
    else
        echo "$path"
    fi
}

if [ -z "$BUILD_VERBOSE" ]; then
    BUILD_VERBOSE=OFF
fi

source $(dirname $(safe_readlink ${BASH_SOURCE[0]}))/common.sh

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

# store repo root as variable
REPO_ROOT=$(safe_readlink $(dirname $(dirname $(safe_readlink $0))))
OLD_CWD=$(safe_readlink .)

pushd "$REPO_ROOT"

if [ -z "$BUILD_TERMUX_DIR" ]; then
    if [ -z "$BUILD_DIR" ]; then
        BUILD_TERMUX_DIR=build_termux
    else
        BUILD_TERMUX_DIR=$BUILD_DIR/build_termux
    fi
fi
BUILD_TERMUX_DIR=$(safe_readlink ${BUILD_TERMUX_DIR})
mkdir -p $BUILD_TERMUX_DIR
pushd "$BUILD_TERMUX_DIR"

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
echo "BUILD_TERMUX_DIR: $BUILD_TERMUX_DIR"
echo "TOOLS_DIR: $TOOLS_DIR"
echo "SOURCE_DIR: $SOURCE_DIR"
echo "INSTALL_DIR: $INSTALL_DIR"

cleanup () {
    if [ -d "${BUILD_TERMUX_DIR}" ]; then
        rm -rf "${BUILD_TERMUX_DIR}"
    fi
}
if [ "$CI" != "" ]; then
    trap cleanup EXIT
fi

echo_status "Compile RabbitIM ......"
cmake "$REPO_ROOT" \
  -DCMAKE_SYSTEM_NAME=Linux \
  -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
  -DCMAKE_VERBOSE_MAKEFILE=${BUILD_VERBOSE} \
  -DCMARK_SHARED=OFF \
  -DCMARK_TESTS=OFF \
  -DCMARK_STATIC=ON \
  -DWITH_CMARK=OFF \
  -DWITH_CMARK_GFM=ON \
  -DENABLE_UPDATE_TRANSLATIONS=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build . --config Release --parallel $(nproc)
cmake --install . --config Release --strip --component DependLibraries
cmake --install . --config Release --strip --component Runtime
cmake --install . --config Release --strip --component Plugin
cmake --install . --config Release --strip --component Application
