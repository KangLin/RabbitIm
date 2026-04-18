#!/bin/bash
# Author: Kang Lin <kl222@126.com>

#See: https://blog.csdn.net/alwaysbefine/article/details/114187380
#set -x
set -e
#set -v

if [ -z "$BUILD_VERBOSE" ]; then
    BUILD_VERBOSE=OFF
fi

INSTALL_BUILD_DEPEND=0

source $(dirname $(readlink -f $0))/common.sh

usage_long() {
    echo "$0 [-h|--help] [-v|--verbose[=0|1]] [--install=<install directory>] [--rabbitcommon<RabbitCommon directory>"
    echo "  --help|-h: Show help"
    echo "  -v|--verbose: Show build verbose"
    echo "  --install-build-depend: Install build depend"
    echo "  --install: Set depend libraries install directory"
    echo "  --rabbitcommon: Install RabbitCommon"
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
    OPTS=help,verbose::,install:,rabbitcommon::,install-build-depend::
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
        --rabbitcommon)
            case "$2" in
                "")
                    RabbitCommon_ROOT=1
                    ;;
                *)
                    RabbitCommon_ROOT="$2"
                    ;;
            esac
            shift 2
            ;;
        --install-build-depend)
            case "$2" in
                "")
                    INSTALL_BUILD_DEPEND=1
                    ;;
                *)
                    INSTALL_BUILD_DEPEND="$2"
                    ;;
            esac
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
REPO_ROOT=$(safe_readlink $(dirname $(dirname $(safe_readlink $0))))
OLD_CWD=$(safe_readlink .)

pushd "$REPO_ROOT"

if [ -n "$BUILD_VERBOSE" ]; then
    export BUILD_VERBOSE=$BUILD_VERBOSE
fi

if [ -n "$RabbitCommon_ROOT" ]; then
    export RabbitCommon_ROOT=$RabbitCommon_ROOT
fi

if [ -z "$BUILD_TYPE" ]; then
    export BUILD_TYPE=Release
fi

if [ -n "$INSTALL_DIR" ]; then
    export INSTALL_DIR=$INSTALL_DIR
    export CMAKE_PREFIX_PATH=${INSTALL_DIR}:${CMAKE_PREFIX_PATH}
fi

BUILD_DEPS=$(/usr/bin/which mk-build-deps)
BUILD_PKG=$(/usr/bin/which dpkg-buildpackage)

if [ -z "$BUILD_DEPS" ] || [ -z "$BUILD_PKG" ]; then
  echo_error "dpkg-buildpackage [$BUILD_PKG] and dpkg-checkbuilddeps [$BUILD_DEPS] required"
  echo_error "Install with 'sudo apt install dpkg-dev'"
  exit 1
fi

create_debian_folder $REPO_ROOT

# Check all dependencies are installed
if [ $INSTALL_BUILD_DEPEND -eq 1 ]; then
    install_debian_depend $REPO_ROOT
    #$BUILD_DEPS -i -t 'apt-get --no-install-recommends --no-install-suggests -y' "debian/control"
fi

# And finally build the package

#fakeroot debian/rules binary

# -p, --sign-command=sign-command
#  When dpkg-buildpackage needs to execute GPG to sign a source
#  control (.dsc) file or a .changes file it will run sign-command
#  (searching the PATH if necessary) instead of gpg (long option since
#  dpkg 1.18.8).  sign-command will get all the arguments that gpg
#  would have gotten. sign-command should not contain spaces or any
#  other shell metacharacters.

# -k, --sign-key=key-id
#  Specify a key-ID to use when signing packages (long option since
#  dpkg 1.18.8).

# -us, --unsigned-source
#  Do not sign the source package (long option since dpkg 1.18.8).

# -ui, --unsigned-buildinfo
#  Do not sign the .buildinfo file (since dpkg 1.18.19).

# -uc, --unsigned-changes
#  Do not sign the .buildinfo and .changes files (long option since
#  dpkg 1.18.8).

# -b  Equivalent to --build=binary or --build=any,all.
# -S  Equivalent to --build=source
# -d, --no-check-builddeps    do not check build dependencies and conflicts.
#     --ignore-builtin-builddeps
#                             do not check builtin build dependencies.

#The -us -uc tell it there is no need to GPG sign the package. the -b is build binary
$BUILD_PKG -us -uc -b

#The -us -uc tell it there is no need to GPG sign the package. the -S is build source package
#$BUILD_PKG -us -uc -S

#$BUILD_PKG -S

# build source and binary package
#$BUILD_PKG -us -uc 

#$BUILD_PKG -b

popd
