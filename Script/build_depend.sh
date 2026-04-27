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

install_gnu_getopt
if [ "$DISTRO" = "macOS" ]; then
    MACOS=1
    setup_macos
else
    MACOS=0
fi

if [ -z "$PACKAGE_TOOL" ]; then
    PACKAGE_TOOL=apt
fi

PACKAGE=
SYSTEM_UPDATE=0
BASE_LIBS=0
DEFAULT_LIBS=0
QT=0
if [ -z "$QT_VERSION" ]; then
    QT_VERSION=6.10.3
fi
RabbitCommon=0
QXMPP=0
QZXING=0
ZXING_CPP=0

# Display detailed usage information
usage_long() {
    cat << EOF
$(basename $0) - Dependency build script

Usage: $0 [OPTION]...

Options:
  -h, --help                        Show this help message
  -v, --verbose[=LEVEL]             Set verbose mode (LEVEL: ON, OFF, default: $BUILD_VERBOSE)

Directory options:
  --install=DIR                     Set installation directory
  --source=DIR                      Set source code directory  
  --tools=DIR                       Set tools directory
  --build=DIR                       Set build directory

Package management options:
  --package="PKG1 PKG2 ..."         Install specified system packages
  --package-tool=TOOL               Set package manager tool (apt, dnf, brew, pacman, zypper, apk)
  --system_update[=1|0]             Update system package manager
  --system-update[=1|0]             Update system package manager

Dependency options:
  --base[=1|0]                      Install basic development libraries
  --default[=1|0]                   Install system default dependency libraries
  --qt[=1|0|VERSION]                Install Qt (can specify version)
  --macos[=1|0]                     Install macOS specific tools and dependencies

Component options:
  --rabbitcommon[=1|0]              Install RabbitCommon
  --qxmpp[=1|0]                     Install QXMPP
  --qzxing[=1|0]                    Install QZXing

Examples:
  $0 --base=1 --qt=1 --install=/opt/local
  $0 --verbose --package="git cmake" --freerdp=1 --tigervnc=1
  $0 --system-update --base --default --qt=6.5.0

Environment variables:
  BUILD_VERBOSE     Set verbose mode (ON/OFF, default: $BUILD_VERBOSE)
  QT_VERSION        Set Qt version (default: $QT_VERSION)
EOF
    exit 0
}

# Parse arguments using getopt (more powerful)
# [如何使用getopt和getopts命令解析命令行选项和参数](https://zhuanlan.zhihu.com/p/673908518)
# [【Linux】Shell命令 getopts/getopt用法详解](https://blog.csdn.net/arpospf/article/details/103381621)
# 
# 注意：在 macOS 上，本地 getopt 不支持长格式参数，所以需要先在系统上安装 GNU getopt，并设置环境变量 PATH
#  brew install gnu-getopt
#  export PATH="/usr/local/opt/gnu-getopt/bin:$PATH"
parse_with_getopt() {
    local OPTS ARGS
    
    echo_status "Using getopt to parse command line arguments ....."
    
    # Define supported options
    # Format: long_option_name: (colon indicates required argument)
    # :: means optional argument, no colon means no argument
    # -o 或 --options 选项后面是可接受的短选项，如 ab:c:: ，表示可接受的短选项为 -a -b -c ，
    # 其中 -a 选项不接参数，-b 选项后必须接参数，-c 选项的参数为可选的
    # 后面没有冒号表示没有参数。后跟有一个冒号表示有参数。跟两个冒号表示有可选参数。
    # -l 或 --long 选项后面是可接受的长选项，用逗号分开，冒号的意义同短选项。
    # -n 选项后接选项解析错误时提示的脚本名字
    OPTS=help,install:,source:,tools:,build:,verbose::,package:,package-tool:,system_update::,system-update::,base::,default::,macos::,qt::,rabbitcommon::,qxmpp::,qzxing::,zxing-cpp::
    
    # Parse arguments using getopt
    # -o: short options
    # -l: long options  
    # -n: script name for error messages
    ARGS=$(getopt -o h,v:: -l "$OPTS" -n "$(basename "$0")" -- "$@")
    if [ $? != 0 ]; then
        echo_error "Error: Command line argument parsing failed" >&2
        usage_long
    fi
    
    # Set positional parameters to parsed arguments
    #将规范化后的命令行参数分配至位置参数（$1,$2,......)
    eval set -- "$ARGS"
    #echo "formatted parameters=[$@]"

    # Process options
    while true; do
        case "$1" in
        --install)
            validate_directory "$2" "Installation"
            INSTALL_DIR="$2"
            shift 2
            ;;
        --source)
            validate_directory "$2" "Source code"
            SOURCE_DIR="$2"
            shift 2
            ;;
        --tools)
            validate_directory "$2" "Tools"
            TOOLS_DIR="$2"
            shift 2
            ;;
        --build)
            validate_directory "$2" "Build"
            BUILD_DIR="$2"
            shift 2
            ;;
        --package)
            PACKAGE="$2"
            shift 2
            ;;
        --package-tool)
            PACKAGE_TOOL="$2"
            shift 2
            ;;
        -v|--verbose)
            case "$2" in
                "")
                    BUILD_VERBOSE="ON"
                    ;;
                *)
                    BUILD_VERBOSE="$2"
                    ;;
            esac
            shift 2
            ;;
        --system_update|--system-update)
            case "$2" in
                "")
                    SYSTEM_UPDATE=1
                    ;;
                *)
                    SYSTEM_UPDATE="$2"
                    ;;
            esac
            shift 2
            ;;
        --base)
            case "$2" in
                "")
                    BASE_LIBS=1
                    ;;
                *)
                    BASE_LIBS="$2"
                    ;;
            esac
            shift 2
            ;;
        --default)
            case "$2" in
                "")
                    DEFAULT_LIBS=1
                    ;;
                *)
                    DEFAULT_LIBS="$2"
                    ;;
            esac
            shift 2
            ;;
        --rabbitcommon)
            case "$2" in
                "")
                    RabbitCommon=1
                    ;;
                *)
                    RabbitCommon="$2"
                    ;;
            esac
            shift 2
            ;;
        --qt)
            case "$2" in
                "")
                    QT=1
                    ;;
                1|0)
                    QT="$2"
                    ;;
                *)
                    QT_VERSION="$2"
                    QT=1
                    ;;
            esac
            shift 2
            ;;
        --macos)
            case "$2" in
                "")
                    MACOS=1
                    ;;
                *)
                    MACOS="$2"
                    ;;
            esac
            shift 2
            ;;
        --qxmpp)
            case "$2" in
                "")
                    QXMPP=1
                    ;;
                *)
                    QXMPP="$2"
                    ;;
            esac
            shift 2
            ;;
        --qzxing)
            case "$2" in
                "")
                    QZXING=1
                    ;;
                *)
                    QZXING="$2"
                    ;;
            esac
            shift 2
            ;;
        --zxing-cpp)
            case "$2" in
                "")
                    ZXING_CPP=1
                    ;;
                *)
                    ZXING_CPP="$2"
                    ;;
            esac
            shift 2
            ;;
        --) # End of options
            shift
            break
            ;;
        -h|--help)
            usage_long
            ;;
        *)
            echo_error "Error: Unknown option '$1'" >&2
            usage_long
            ;;
        esac
    done
    
    # Handle remaining non-option arguments (if any)
    if [ $# -gt 0 ]; then
        echo_warn "Warning: Ignoring unknown arguments: $*" >&2
    fi
}

# Parse command line arguments
parse_command_line() {
    # Use getopt if available, otherwise fall back to getopts
    if command -v getopt >/dev/null 2>&1; then
        parse_with_getopt "$@"
    else
        echo_error "Please install GNU getopt"
        exit 1
    fi
}

# Display current configuration
show_configuration() {
    if [ "$BUILD_VERBOSE" = "ON" ]; then
        echo "=== Current Configuration ==="
        echo "Directory Configuration:"
        echo "  Install Directory: ${INSTALL_DIR:-Not set (using default)}"
        echo "  Source Directory: ${SOURCE_DIR:-Not set (using default)}"
        echo "  Tools Directory: ${TOOLS_DIR:-Not set (using default)}"
        echo "  Build Directory: ${BUILD_DIR:-Not set (using default)}"
        echo "  Build Depend Directory: ${BUILD_DEPEND_DIR:-Not set (using default)}"
        echo ""
        echo "Package Management:"
        echo "  System Update: $SYSTEM_UPDATE"
        echo "  Package Installation: ${PACKAGE:-None}"
        echo "  Package Tool: ${PACKAGE_TOOL:-Auto-detected}"
        echo ""
        echo "Dependency Installation:"
        echo "  Base Libraries: $BASE_LIBS"
        echo "  Default Libraries: $DEFAULT_LIBS"
        echo "  Qt: $QT (Version: $QT_VERSION)"
        echo "  macOS Tools: $MACOS"
        echo ""
        echo "Component Installation:"
        echo "  RabbitCommon: $RabbitCommon"
        echo "  QXMPP: $QXMPP"
        echo "  QZXing: $QZXing"
        echo "  ZXing: $ZXING_CPP"
        echo ""
        echo "Other Settings:"
        echo "  Verbose Mode: $BUILD_VERBOSE"
        echo "========================="
        echo ""
    fi

    echo "Repo folder: $REPO_ROOT"
    echo "Old folder: $OLD_CWD"
    echo "Current folder: `pwd`"
}

# Validate parsed parameters
validate_parameters() {
    # Check if any action was requested
    local actions=0
    for action in $SYSTEM_UPDATE $BASE_LIBS $DEFAULT_LIBS $QT $QXMPP $QZXING; do
        if [ "$action" = "1" ]; then
            actions=1
            break
        fi
    done
    
    # If no actions and no packages specified, show error
    if [ $actions -eq 0 ] && [ -z "$PACKAGE" ]; then
        echo_warn "Warning: No operation specified" >&2
        #echo "Use '$0 --help' to see available options" >&2
    fi
    
    # Validate boolean parameters
    for var in SYSTEM_UPDATE BASE_LIBS DEFAULT_LIBS QT QXMPP QZXING; do
        local value="${!var}"
        if [ "$value" != "0" ] && [ "$value" != "1" ]; then
            echo_error "Error: Parameter $var must be 0 or 1" >&2
            exit 1
        fi
    done
    
    # Validate verbose mode
    if [ "$BUILD_VERBOSE" != "ON" ] && [ "$BUILD_VERBOSE" != "OFF" ]; then
        echo "Error: BUILD_VERBOSE must be ON or OFF" >&2
        exit 1
    fi
}

# Parse command line arguments (will override environment variables)
parse_command_line "$@"

# store repo root as variable
REPO_ROOT=$(safe_readlink $(dirname $(dirname $(safe_readlink $0))))
OLD_CWD=$(safe_readlink .)

pushd "$REPO_ROOT"

if [ -z "$BUILD_DEPEND_DIR" ]; then
    if [ -z "$BUILD_DIR" ]; then
        BUILD_DEPEND_DIR=build_depend
    else
        BUILD_DEPEND_DIR=$BUILD_DIR/build_depend
    fi
fi
BUILD_DEPEND_DIR=$(safe_readlink ${BUILD_DEPEND_DIR})
mkdir -p $BUILD_DEPEND_DIR
pushd "$BUILD_DEPEND_DIR"

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

# Validate parameters
validate_parameters

# Display configuration
show_configuration

# Set libraries install path
case "$DISTRO" in
ubuntu|debian)
    LIB_PATH="lib"
    ;;
fedora)
    LIB_PATH="lib64"
    ;;
esac

if [ $SYSTEM_UPDATE -eq 1 ]; then
    echo_status "System update ......"
    case "$PACKAGE_TOOL" in
        brew)
            brew update -q
            ;;
        apt)
            if [ "$BUILD_VERBOSE" = "ON" ]; then
                apt update -y
            else
                apt update -y -qq
            fi
            ;;
        *)
            "$PACKAGE_TOOL" update -y
            ;;
    esac
fi

if [ -n "$PACKAGE" ]; then
    echo_status "Install package: $PACKAGE"
    package_install $PACKAGE
fi

if [ $BASE_LIBS -eq 1 ]; then
    echo_status "Install base libraries ......"
    if [ "$PACKAGE_TOOL" = "apt" ]; then
        # Build tools
        package_install build-essential devscripts equivs debhelper \
            fakeroot graphviz gettext wget curl git cmake ninja-build doxygen \
            dh-make lintian quilt git-buildpackage dctrl-tools
            #pbuilder

        # OpenGL
        #package_install libgl1-mesa-dev libglx-dev libglu1-mesa-dev libvulkan-dev mesa-common-dev
        # Virtual desktop (virtual framebuffer X server for X Version 11). Needed by CI
        if [ -z "$RabbitIm_VERSION" ]; then
            package_install xvfb #xpra
        fi
        # RabbitCommon dependency
        package_install libcmark-dev
        # FFmpeg needed by QtMultimedia
        package_install libavcodec-dev libavformat-dev libswscale-dev
        case "$DISTRO" in
        ubuntu|debian)
            package_install libresample1-dev
            ;;
        deepin)
            package_install libswresample-dev
            ;;
        esac
        # Needed by AppImage
        package_install fuse3
    fi

    if [ "$PACKAGE_TOOL" = "dnf" ]; then
        package_install make git rpm-build rpmdevtools gcc-c++ util-linux \
           automake autoconf libtool gettext gettext-autopoint \
           cmake desktop-file-utils curl wget dnf-plugins-core
    fi

    if [ $MACOS -eq 1 ]; then
        package_install nasm autoconf automake libtool pkg-config doxygen zstd curl
    fi
fi

if [ $DEFAULT_LIBS -eq 1 ]; then
    echo_status "Install default dependency libraries ......"
    if [ "$PACKAGE_TOOL" = "apt" ]; then

        package_install qmake6 qt6-tools-dev qt6-tools-dev-tools \
            qt6-base-dev qt6-base-dev-tools qt6-qpa-plugins \
            qt6-svg-dev qt6-l10n-tools qt6-translations-l10n \
            qt6-scxml-dev qt6-multimedia-dev qt6-positioning-dev \
            libqt6sql6-mysql libqt6sql6-sqlite libqt6sql6-odbc libqt6sql6-psql \
            qt6-speech-dev

    fi # apt

    if [ "$PACKAGE_TOOL" = "dnf" ]; then
        if [ $QT -ne 1 ]; then
            package_install qt6-qttools-devel qt6-qtbase-devel qt6-qtmultimedia-devel \
                qt6-qt5compat-devel qt6-qtmultimedia-devel qt6-qtscxml-devel \
                qt6-qtsvg-devel qt6-qtpositioning-devel
        fi

        dnf builddep -y ${REPO_ROOT}/Package/rpm/rabbitim.spec
    fi

    if [ $MACOS -eq 1 ]; then
        package_install qt
    fi
fi

if [ $QT -eq 1 ]; then
    echo_status "Install qt ${QT_VERSION} ......"
    pushd "$TOOLS_DIR"
    if [ ! -d qt_`uname -m` ]; then
        # See: https://ddalcino.github.io/aqt-list-server/
        #      https://www.cnblogs.com/clark1990/p/17942952
        #if [ "$PACKAGE_TOOL" = "apt" ]; then
        #    package_install python3-pip python3-pip-whl python3-pipdeptree cpio
        #    pip install --upgrade typing-extensions
        #fi

        #pip install -U pip
        pip install --ignore-installed aqtinstall

        echo "PATH: $PATH"
        if [ "`uname -m`" == "x86_64" ]; then
            aqt install-qt linux desktop ${QT_VERSION} linux_gcc_64 -m qtscxml qtmultimedia qtimageformats qtserialport qt5compat qtwebsockets qtpositioning qtwebchannel qtwebengine qtspeech qtvirtualkeyboard
            mv ${QT_VERSION}/gcc_64 qt_`uname -m`
         elif [ "`uname -m`" == "aarch64" ]; then
            aqt install-qt linux_arm64 desktop ${QT_VERSION} linux_gcc_arm64 -m qtscxml qtmultimedia qtimageformats qtserialport qt5compat qtwebsockets qtpositioning qtwebchannel qtwebengine qtspeech qtvirtualkeyboard
            mv ${QT_VERSION}/gcc_arm64 qt_`uname -m`
        fi
    fi
    popd
fi

if [ $RabbitCommon -eq 1 ]; then
    echo_status "Install RabbitCommon ......"
    pushd "$SOURCE_DIR"
    if [ ! -d RabbitCommon ]; then
        git clone https://github.com/KangLin/RabbitCommon.git
    else
        pushd RabbitCommon
        git pull
        popd
    fi
    popd
fi

if [ $QXMPP -eq 1 ]; then
    if [ $BASE_LIBS -eq 1 ]; then
        echo_status "Install QXMPP dependencies ......"
        # Needed by qxmpp
        package_install libssl-dev libomemo-c-dev \
            libgstreamermm-1.0-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
    fi

    echo_status "Install QXMPP ......"
    pushd "$SOURCE_DIR"

    QXMPP_INSTALL_DIR="${INSTALL_DIR}/${LIB_PATH}/cmake/QXmppQt6"
    QXMPP_VERSION=v1.14.7

    if [ ! -d ${QXMPP_INSTALL_DIR} ]; then
        if [ ! -d qxmpp ]; then
           git clone -b ${QXMPP_VERSION} --depth=1 https://invent.kde.org/libraries/qxmpp.git
        fi
        cmake -E make_directory $BUILD_DEPEND_DIR/qxmpp
        cd $BUILD_DEPEND_DIR/qxmpp
        cmake -S ${SOURCE_DIR}/qxmpp -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
            -DCMAKE_VERBOSE_MAKEFILE=${BUILD_VERBOSE} \
            -DBUILD_DOCUMENTATION=OFF \
            -DBUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF
        cmake --build . --config Release --parallel $(nproc)
        cmake --install . --config Release
    fi
    popd
fi

if [ $QZXING -eq 1 ]; then
    echo_status "Install qzxing ......"
    pushd "$SOURCE_DIR"
    if [ ! -d ${INSTALL_DIR}/${LIB_PATH}/cmake/qzxing ]; then
        if [ ! -d qzxing ]; then
            git clone --depth=1 https://github.com/KangLin/qzxing.git
        fi
        cmake -E make_directory $BUILD_DEPEND_DIR/qzxing
        pushd $BUILD_DEPEND_DIR/qzxing
        cmake -S $SOURCE_DIR/qzxing -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
            -DCMAKE_VERBOSE_MAKEFILE=${BUILD_VERBOSE}
        cmake --build . --config Release --parallel $(nproc)
        cmake --install . --config Release
        popd
    fi
    popd
fi

if [ $ZXING_CPP -eq 1 ]; then
    echo_status "Install zxing-cpp ......"
    pushd "$SOURCE_DIR"
    if [ ! -d ${INSTALL_DIR}/${LIB_PATH}/cmake/ZXing ]; then
        if [ ! -d zxing-cpp ]; then
            git clone -b v3.0.2 --recursive --depth=1 https://github.com/zxing-cpp/zxing-cpp.git
        fi
        cmake -E make_directory $BUILD_DEPEND_DIR/zxing-cpp
        pushd $BUILD_DEPEND_DIR/zxing-cpp
        cmake -S $SOURCE_DIR/zxing-cpp -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
            -DCMAKE_VERBOSE_MAKEFILE=${BUILD_VERBOSE} \
            -DZXING_EXAMPLES=OFF
        cmake --build . --config Release --parallel $(nproc)
        cmake --install . --config Release
        popd
    fi
    popd
fi

popd
popd
