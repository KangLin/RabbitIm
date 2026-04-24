#!/bin/bash

set -e
#set -x

if [ -z "$BUILD_VERBOSE" ]; then
    BUILD_VERBOSE=OFF
fi

source $(dirname $(readlink -f $0))/common.sh

install_gnu_getopt
if [ "$OS" = "macOS" ]; then
    MACOS=1
    setup_macos
else
    MACOS=0
fi

DOCKER=0
DEB=0
RPM=0
APPIMAGE=0
DEFAULT_QT=

if [ -z "$QT_VERSION" ]; then
    QT_VERSION=6.10.3
fi

# Display detailed usage information
usage_long() {
    cat << EOF
$(basename $0) - Build script

Usage: $0 [OPTION]...

Options:
  -h, --help            Show this help message
  -v, --verbose[=LEVEL] Set verbose mode (LEVEL: ON, OFF, default: $BUILD_VERBOSE)

Directory options:
  --install=DIR         Set installation directory
  --source=DIR          Set source code directory  
  --tools=DIR           Set tools directory
  --build=DIR           Set build directory

Docker options:
  --docker:             Run docket for build
  --docker-image:       The name of docker image
  --docker-platform     The docker image platform

Target options:
  --deb:                Build deb package
  --rpm:                Build rpm package
  --appimage:           Build AppImage
  --macos:              Build macOS
  
Other options:
  --qt=VERSION    Install Qt (can specify version)(only --appimage)

Dependency options:
  --qt5[=1|0]               Install system default qt5 libraries
  --qt6[=1|0]               Install system default qt6 libraries

Examples:
  $0 --base=1 --qt=$QT_VERSION --install=/opt/local -deb
  $0 --verbose --docker --docker-image=$DISTRO:$DISTRO_VERSION --appimage --qt=$QT_VERSION

Environment variables:
  BUILD_VERBOSE     Set verbose mode (ON/OFF, default: $BUILD_VERBOSE)
  QT_VERSION        Set Qt version (default: $QT_VERSION)
EOF
    exit 0
}

parse_with_getopt() {
    # [如何使用 getopt 和 getopts 命令解析命令行选项和参数](https://zhuanlan.zhihu.com/p/673908518)
    # [【Linux】Shell 命令 getopts/getopt 用法详解](https://blog.csdn.net/arpospf/article/details/103381621)
    if command -V getopt >/dev/null; then
        echo "getopt is exits"
        #echo "original parameters=[$@]"
        # -o 或 --options 选项后面是可接受的短选项，如 ab:c:: ，表示可接受的短选项为 -a -b -c ，
        # 其中 -a 选项不接参数，-b 选项后必须接参数，-c 选项的参数为可选的
        # 后面没有冒号表示没有参数。后跟有一个冒号表示有参数。跟两个冒号表示有可选参数。
        # -l 或 --long 选项后面是可接受的长选项，用逗号分开，冒号的意义同短选项。
        # -n 选项后接选项解析错误时提示的脚本名字
        OPTS=help,verbose::,docker::,deb::,rpm::,appimage::,macos::,docker-image:,docker-platform::,qt:,qt5:,qt6:,install:,source:,tools:,build:
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
                BUILD_DIR="$2"
                shift 2
                ;;
            -v|--verbose)
                case $2 in
                    "")
                        BUILD_VERBOSE=ON;;
                    *)
                        BUILD_VERBOSE=$2;;
                esac
                shift 2
                ;;
            --docker)
                case $2 in
                    "")
                        DOCKER=1;;
                    *)
                        DOCKER=$2;;
                esac
                shift 2
                ;;
            --docker-image)
                case $2 in
                    *)
                        DOCKER_IMAGE=$2
                        DOCKER=1;;
                esac
                shift 2
                ;;
            --docker-platform)
                case $2 in
                    "")
                        ;;
                    *)
                        DOCKER_PLATFORM=$2
                        DOCKER=1;;
                esac
                shift 2
                ;;
            --deb)
                case $2 in
                    "")
                        DEB=1;;
                    *)
                        DEB=$2;;
                esac
                shift 2
                ;;
            --rpm)
                case $2 in
                    "")
                        RPM=1;;
                    *)
                        RPM=$2;;
                esac
                shift 2
                ;;
            --appimage)
                case $2 in
                    "")
                        APPIMAGE=1;;
                    *)
                        APPIMAGE=$2;;
                esac
                shift 2
                ;;
            --macos)
                case $2 in
                    "")
                        MACOS=1;;
                    *)
                        MACOS=$2;;
                esac
                shift 2
                ;;
            --qt)
                case $2 in
                    *)
                        QT_VERSION=$2;;
                esac
                shift 2
                ;;
            --qt5)
                case "$2" in
                    *)
                        DEFAULT_QT="--default-qt5"
                        ;;
                esac
                shift 2
                ;;
            --qt6)
                case "$2" in
                    *)
                        DEFAULT_QT="--default-qt6"
                        ;;
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
}

# Parse command line arguments
parse_command_line() {
    # Use getopt if available, otherwise fall back to getopts
    if command -v getopt >/dev/null 2>&1; then
        parse_with_getopt "$@"
    else
        echo "Install GNU getopt"
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
        echo "Use Docker: $DOCKER"
        echo "    Docker image: ${DOCKER_IMAGE:-Not set (using default)}"
        echo ""
        echo "Build target:"
        echo "  DEB Package: $DEB"
        echo "  RPM Package: $RPM"
        echo "  AppImage Package: $APPIMAGE"
        echo ""
        echo "Component Installation:"
        echo "  Qt: $QT_VERSION"
        echo ""
        echo "Other Settings:"
        echo "  Verbose Mode: $BUILD_VERBOSE"
        echo "========================="
        echo ""
    fi

    echo "Repo folder: $REPO_ROOT"
    echo "Old folder: $OLD_CWD"
    echo "Current folder: `pwd`"
    echo "SOURCE_DIR: $SOURCE_DIR"
    echo "TOOLS_DIR: $TOOLS_DIR"
    echo "INSTALL_DIR: $INSTALL_DIR"
    echo "BUILD_DIR: $BUILD_DIR"
    echo "BUILD_LINUX_DIR: $BUILD_LINUX_DIR"
}

validate_parameters() {

    # Validate boolean parameters
    for var in DEB RPM DOCKER APPIMAGE; do
        local value="${!var}"
        if [ "$value" != "0" ] && [ "$value" != "1" ]; then
            echo_error "Error: Parameter $var must be 0 or 1" >&2
            exit 1
        fi
    done

    # Validate build target in OS
    case "$DOCKER_IMAGE" in
        "")
            ;;
        ubuntu*|debian*|kali*|*kylin*|*deepin*)
            if [ $RPM -eq 1 ]; then
              echo_error "Error: Not recommended build rpm package in $DOCKER_IMAGE"
              exit 1
            fi
            ;;
        fedora*|rhel*|centos*|almalinux*|rocky*)
            if [ $DEB -eq 1 ]; then
              echo_error "Error: Not recommended build deb package in $DOCKER_IMAGE"
              exit 1
            fi
            ;;
        *)
            if [ $RPM -eq 1 ]; then
              echo_error "Error: Not recommended build rpm package in $DOCKER_IMAGE"
              exit 1
            fi
            if [ $DEB -eq 1 ]; then
              echo_error "Error: Not recommended build deb package in $DOCKER_IMAGE"
              exit 1
            fi
        ;;
    esac
}

# Parse command line arguments (will override environment variables)
parse_command_line "$@"

## Basic variables
if [ $DOCKER -eq 1 ] && [ -z "$DOCKER_IMAGE" ]; then
    DOCKER_IMAGE=$DISTRO
fi

## Directory variables
CURDIR=$(dirname $(safe_readlink $0))
REPO_ROOT=$(safe_readlink $(dirname $(dirname $(safe_readlink $0))))

if [ -z "$BUILD_LINUX_DIR" ]; then
    if [ -z "$BUILD_DIR" ]; then
        BUILD_LINUX_DIR=${REPO_ROOT}/build_linux
    else
        BUILD_LINUX_DIR=${BUILD_DIR}/build_linux
    fi
fi
if [ -z "$SOURCE_DIR" ]; then
    SOURCE_DIR=${BUILD_LINUX_DIR}/source
fi
if [ -z "$TOOLS_DIR" ]; then
    TOOLS_DIR=${BUILD_LINUX_DIR}/tools
fi
if [ -z "$INSTALL_DIR" ]; then
    INSTALL_DIR=${BUILD_LINUX_DIR}/install
fi
#rm -fr $SOURCE_DIR
mkdir -p $SOURCE_DIR
#rm -fr $TOOLS_DIR
mkdir -p $TOOLS_DIR
#rm -fr $INSTALL_DIR
mkdir -p $INSTALL_DIR

# Validate parameters
validate_parameters

# Display configuration
show_configuration

if [ $DOCKER -eq 1 ]; then
    echo_status "Start docker ${DOCKER_IMAGE} ......"
    ARCH=`dpkg --print-architecture`
    echo "The host arch: $ARCH"
    if [ -z "$DOCKER_IMAGE" ]; then
        echo_error "DOCKER_IMAGE is empty. please set --docker-image"
        exit 1
    fi
    ## Copy the source code to build directory
    pushd ${REPO_ROOT}
    # Generated source archive
    git archive --format=tar.gz -o RabbitIm.tar.gz --prefix RabbitIm/ HEAD
    if [ -d ${BUILD_LINUX_DIR} ]; then
        rm -fr ${BUILD_LINUX_DIR}
    fi
    mkdir -p ${BUILD_LINUX_DIR}
    mv RabbitIm.tar.gz ${BUILD_LINUX_DIR}/
    chmod a+rw ${BUILD_LINUX_DIR}/RabbitIm.tar.gz
    popd

    if [ $DEB -eq 1 ]; then
        if [[ "$DOCKER_IMAGE" =~ ^(ubuntu|debian) ]]; then
            DOCKER_PARA="-e DEBIAN_FRONTEND=noninteractive -e TZ=UTC"
        fi
        echo "DOCKER_PLATFORM: $DOCKER_PLATFORM"
        if [ -n "$DOCKER_PLATFORM" ]; then
            DOCKER_PARA="$DOCKER_PARA --platform $DOCKER_PLATFORM"
        fi
        docker run --privileged ${DOCKER_PARA} \
            -e CI=${CI} \
            --volume ${REPO_ROOT}:/home/RabbitIm \
            --volume ${BUILD_LINUX_DIR}:/home/build \
            --volume ${INSTALL_DIR}:/home/install \
            --volume ${TOOLS_DIR}:/home/tools \
            --interactive --rm ${DOCKER_IMAGE} \
           bash -e -x -c "
           if [ ! \$CI ]; then
               tar -C ~ -xf /home/build/RabbitIm.tar.gz
               export SOURCE_CODE_DIR=~
           else
               export SOURCE_CODE_DIR=/home
           fi
           \${SOURCE_CODE_DIR}/RabbitIm/Script/build_linux.sh --deb --install=/home/install --tools=/home/tools --verbose=${BUILD_VERBOSE} ${DEFAULT_QT}
           cp \${SOURCE_CODE_DIR}/rabbitim*.deb /home/build/
           "
    fi

    if [ $APPIMAGE -eq 1 ]; then
        #if [[ "$DOCKER_IMAGE" =~ ^(ubuntu|debian) ]]; then
        #    DOCKER_PARA="-e DEBIAN_FRONTEND=noninteractive -e TZ=UTC"
        #fi
        case "$DISTRO" in
        ubuntu|debian)
            DOCKER_PARA="-e DEBIAN_FRONTEND=noninteractive -e TZ=UTC"
            ;;
        fedora)
            # Install getopt
            dnf install -y util-linux
            ;;
        esac
        docker run --privileged ${DOCKER_PARA} \
            -e CI=${CI} \
            --volume ${REPO_ROOT}:/home/RabbitIm \
            --volume ${BUILD_LINUX_DIR}:/home/build \
            --volume ${INSTALL_DIR}:/home/install \
            --volume ${TOOLS_DIR}:/home/tools \
            --interactive --rm ${DOCKER_IMAGE} \
            bash -e -x -c "
            if [ ! \$CI ]; then
                tar -C ~ -xf /home/build/RabbitIm.tar.gz
                export SOURCE_CODE_DIR=~
            else
                export SOURCE_CODE_DIR=/home
            fi
            \${SOURCE_CODE_DIR}/RabbitIm/Script/build_linux.sh --appimage --install=/home/install --tools=/home/tools --verbose=${BUILD_VERBOSE} ${DEFAULT_QT}
            # Create install script
            echo \"== Create install script ......\"
            mkdir -p /home/build/install
            pushd /home/build/install
            cp \${SOURCE_CODE_DIR}/RabbitIm/RabbitIm_`uname -m`.AppImage .
            chmod a+rx RabbitIm_`uname -m`.AppImage
            cp \${SOURCE_CODE_DIR}/RabbitIm/build_appimage/AppDir/usr/share/applications/io.github.KangLin.RabbitIm.desktop .
            cp \${SOURCE_CODE_DIR}/RabbitIm/build_appimage/AppDir/usr/share/pixmaps/io.github.KangLin.RabbitIm.png .
            cp \${SOURCE_CODE_DIR}/RabbitIm/Script/install_appimage.sh install.sh
            chmod a+rx install.sh
            popd
            "
    fi

    if [ $RPM -eq 1 ]; then
        docker run --volume ${BUILD_LINUX_DIR}:/home/build \
            --volume ${INSTALL_DIR}:/home/install \
            --volume ${TOOLS_DIR}:/home/tools \
            --privileged --interactive --rm ${DOCKER_IMAGE} \
            bash -e -x -c "
            mkdir -p ~/rpmbuild/SOURCES/
            cp /home/build/RabbitIm.tar.gz ~/rpmbuild/SOURCES/
            tar -C ~ -xf /home/build/RabbitIm.tar.gz
            # Install getopt
            dnf install -y util-linux
            ~/RabbitIm/Script/build_linux.sh --rpm --install=/home/install --tools=/home/tools --verbose=${BUILD_VERBOSE}
            cp ~/rpmbuild/RPMS/`uname -m`/rabbitim*.rpm /home/build/
            "
    fi
    exit 0
fi

pushd $REPO_ROOT/Script

if [ $DEB -eq 1 ]; then
    echo_status "build deb package ......"

    if [ "$DEFAULT_QT"="--default-qt5" ]; then
        depend_para="--qzxing"
        export CMAKE_CONFIG_PARAS="$CMAKE_CONFIG_PARAS -DINSTALL_QZXING=ON"
    else
        depend_para="--zxing-cpp"
    fi
    ./build_depend.sh --system_update --base --default ${DEFAULT_QT} \
        --rabbitcommon ${depend_para} \
        --install=${INSTALL_DIR} \
        --source=${SOURCE_DIR} \
        --tools=${TOOLS_DIR} \
        --verbose=${BUILD_VERBOSE}

    install_debian_depend $REPO_ROOT

    # Disable ci warn
    if [ $CI ]; then
        git config --global --add safe.directory $REPO_ROOT
    fi

    export CMAKE_CONFIG_PARAS="$CMAKE_CONFIG_PARAS "
    ./build_debpackage.sh --install=${INSTALL_DIR} \
        --rabbitcommon=${SOURCE_DIR}/RabbitCommon \
        --verbose=${BUILD_VERBOSE}
fi

if [ $APPIMAGE -eq 1 ]; then
    echo_status "build AppImage ......"
    case "$DISTRO" in
    ubuntu)
#        case "$DISTRO_VERSION" in
#            "24.04"|"24.10")
#                depend_para="--qt=${QT_VERSION}"
#                export QT_ROOT=${TOOLS_DIR}/qt_`uname -m`
#                export Qt6_DIR=$QT_ROOT
#                export QMAKE=$QT_ROOT/bin/qmake
#                export QT_PLUGIN_PATH=$QT_ROOT/plugins
#                export PATH=$QT_ROOT/libexec:$PATH
#                export CMAKE_PREFIX_PATH=$QT_ROOT:${INSTALL_DIR}:${CMAKE_PREFIX_PATH}
#                ;;
#        esac
        LIB_PATH="lib"
        depend_para="$depend_para "
        ;;
    fedora)
        LIB_PATH="lib64"
        depend_para="$depend_para --package-tool=dnf  "
        #dnf builddep -y ${REPO_ROOT}/Package/rpm/RabbitIm.spec
        ;;
    *)
    esac

    if [ "$DEFAULT_QT"="--default-qt5" ]; then
        depend_para="--qzxing"
        export CMAKE_CONFIG_PARAS="$CMAKE_CONFIG_PARAS -DINSTALL_QZXING=ON"
    else
        depend_para="--zxing-cpp"
    fi
    export RabbitCommon_ROOT=${SOURCE_DIR}/RabbitCommon
    export BUILD_FREERDP=ON
    export PKG_CONFIG_PATH=${INSTALL_DIR}/${LIB_PATH}/pkgconfig:$PKG_CONFIG_PATH
    export LD_LIBRARY_PATH=${INSTALL_DIR}/${LIB_PATH}:$LD_LIBRARY_PATH
    export CMAKE_PREFIX_PATH=${INSTALL_DIR}:${CMAKE_PREFIX_PATH}

    ./build_depend.sh --system_update --base --default ${DEFAULT_QT} \
        --rabbitcommon ${depend_para} \
        --install=${INSTALL_DIR} \
        --source=${SOURCE_DIR} \
        --tools=${TOOLS_DIR} \
        --verbose=${BUILD_VERBOSE}

    # Disable ci warn
    if [ $CI ]; then
        git config --global --add safe.directory $REPO_ROOT
    fi

    ./build_appimage.sh --install=${INSTALL_DIR} \
        --tools=${TOOLS_DIR} \
        --verbose=${BUILD_VERBOSE}
fi

if [ $RPM -eq 1 ]; then
    echo_status "build rpm package ......"
    #dnf builddep -y ${REPO_ROOT}/Package/rpm/RabbitIm.spec
    if [ "$DEFAULT_QT"="--default-qt5" ]; then
        depend_para="--qzxing"
        export CMAKE_CONFIG_PARAS="$CMAKE_CONFIG_PARAS -DINSTALL_QZXING=ON"
    else
        depend_para="--zxing-cpp"
    fi
    ./build_depend.sh --system_update --base --default --package-tool=dnf \
        --rabbitcommon ${depend_para} \
        --install=${INSTALL_DIR} \
        --source=${SOURCE_DIR} \
        --tools=${TOOLS_DIR} \
        --verbose=${BUILD_VERBOSE}

    ./build_rpm_package.sh \
        --install=${INSTALL_DIR} \
        --source=${SOURCE_DIR} \
        --tools=${TOOLS_DIR} \
        --verbose=${BUILD_VERBOSE}
fi

if [ $MACOS -eq 1 ]; then
    echo_status "build macos bundle package ......"
    ./build_depend.sh --system_update --base --default --macos \
        --rabbitcommon --tigervnc --qtermwidget --qftpserver \
        --install=${INSTALL_DIR} \
        --source=${SOURCE_DIR} \
        --tools=${TOOLS_DIR} \
        --verbose=${BUILD_VERBOSE}

    export RabbitCommon_ROOT=${SOURCE_DIR}/RabbitCommon

    # Disable ci warn
    if [ $CI ]; then
        git config --global --add safe.directory $REPO_ROOT
    fi

    ./build_macos.sh --install=${INSTALL_DIR} \
        --source=${SOURCE_DIR} \
        --tools=${TOOLS_DIR} \
        --verbose=${BUILD_VERBOSE}
fi

popd
