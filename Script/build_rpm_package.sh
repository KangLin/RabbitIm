#!/bin/bash
# Author: Kang Lin <kl222@126.com>

#See: https://blog.csdn.net/alwaysbefine/article/details/114187380
#set -x
set -e
#set -v

if [ -z "$BUILD_VERBOSE" ]; then
    BUILD_VERBOSE=OFF
fi

source $(dirname $(readlink -f $0))/common.sh

usage_long() {
    echo "$0 [-h|--help] [-v|--verbose[=0|1]] [--install=<install directory>]"
    echo "  --help|-h: Show help"
    echo "  -v|--verbose: Show build verbose"
    echo "Directory:"
    echo "  --install: Set depend libraries install directory"
    echo "  --source: Set source directory"
    echo "  --tools: Set tools directory"
    echo "  --build: Set build directory"
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
            BUILD_RPM_DIR=$2
            shift 2
            ;;
        -v |--verbose)
            case $2 in
                "")
                    BUILD_VERBOSE=ON;;
                *)
                    BUILD_VERBOSE=$2;;
            esac
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
SCRIPT_DIR=$(dirname $(safe_readlink $0))
REPO_ROOT=$(safe_readlink $(dirname $(dirname $(safe_readlink $0))))
OLD_CWD=$(safe_readlink .)

if [ -z "$BUILD_RPM_DIR" ]; then
    BUILD_RPM_DIR=${REPO_ROOT}/build_rpm
fi
if [ -z "$SOURCE_DIR" ]; then
    SOURCE_DIR=${BUILD_RPM_DIR}/source
fi
if [ -z "$TOOLS_DIR" ]; then
    TOOLS_DIR=${BUILD_RPM_DIR}/tools
fi
if [ -z "$INSTALL_DIR" ]; then
    INSTALL_DIR=${BUILD_RPM_DIR}/install
fi

mkdir -p $(safe_readlink $BUILD_RPM_DIR)
TOOLS_DIR=$(safe_readlink ${TOOLS_DIR})
mkdir -p $TOOLS_DIR
SOURCE_DIR=$(safe_readlink ${SOURCE_DIR})
mkdir -p $SOURCE_DIR
INSTALL_DIR=$(safe_readlink ${INSTALL_DIR})
mkdir -p $INSTALL_DIR

echo "Repo folder: $REPO_ROOT"
echo "Script folder: $SCRIPT_DIR"
echo "Old folder: $OLD_CWD"
echo "Current folder: `pwd`"
echo "TOOLS_DIR: $TOOLS_DIR"
echo "SOURCE_DIR: $SOURCE_DIR"
echo "INSTALL_DIR: $INSTALL_DIR"

pushd $REPO_ROOT

if [ ! -f ~/rpmbuild/SOURCES/RabbitIm.tar.gz ]; then
    mkdir -p ~/rpmbuild/SOURCES/
    git archive --format=tar.gz --prefix=RabbitIm/ -o ~/rpmbuild/SOURCES/RabbitIm.tar.gz HEAD
fi
if [ -z "$RabbitCommon_ROOT" ]; then
    if [ -d ${SOURCE_DIR}/RabbitCommon ]; then
        export RabbitCommon_ROOT=${SOURCE_DIR}/RabbitCommon
    elif [ -d ${REPO_ROOT}/../RabbitCommon ]; then
        export RabbitCommon_ROOT=${REPO_ROOT}/../RabbitCommon
    fi
fi
export CMAKE_PREFIX_PATH=${INSTALL_DIR}:${CMAKE_PREFIX_PATH}
export INSTALL_DIR=${INSTALL_DIR}
rpmbuild --nodebuginfo -bb Package/rpm/rabbitim.spec

popd
