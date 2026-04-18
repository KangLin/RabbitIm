#!/bin/bash

# Setup AppImage shell script
# Author: Kang Lin <kl222@126.com>

#set -v
set -e
#set -x

APP_ID=io.github.KangLin.RabbitIm
INSTALL_DIR=$HOME/AppImage/$APP_ID
DESKTOP_FILE_DIR=$HOME/.local/share/applications
DESKTOP_FILE=$DESKTOP_FILE_DIR/$APP_ID.AppImage.desktop

usage_long() {
    echo "$0 [-h|--help] [--install=<install directory>]"
    echo "  -h|--help: show help"
    echo "Directory:"
    echo "  --install: Set install directory"
    exit
}

# [如何使用getopt和getopts命令解析命令行选项和参数](https://zhuanlan.zhihu.com/p/673908518)
# [【Linux】Shell命令 getopts/getopt用法详解](https://blog.csdn.net/arpospf/article/details/103381621)
if command -V getopt >/dev/null; then
    #echo "getopt is exits"
    #echo "original parameters=[$@]"
    # -o 或 --options 选项后面是可接受的短选项，如 ab:c:: ，表示可接受的短选项为 -a -b -c ，
    # 其中 -a 选项不接参数，-b 选项后必须接参数，-c 选项的参数为可选的
    # 后面没有冒号表示没有参数。后跟有一个冒号表示有参数。跟两个冒号表示有可选参数。
    # -l 或 --long 选项后面是可接受的长选项，用逗号分开，冒号的意义同短选项。
    # -n 选项后接选项解析错误时提示的脚本名字
    OPTS=help,install:
    ARGS=`getopt -o h,v:: -l $OPTS -n $(basename $0) -- "$@"`
    if [ $? != 0 ]; then
        echo "exec getopt fail: $?"
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

# 安全的 readlink 函数，兼容各种情况
safe_readlink() {
    local path="$1"
    if [ -L "$path" ]; then
        # 如果是符号链接，使用 readlink
        if command -v readlink >/dev/null 2>&1; then
            if readlink -f "$path" >/dev/null 2>&1; then
                readlink -f "$path"
            else
                readlink "$path"
            fi
        else
            # 如果没有 readlink，使用 ls
            ls -l "$path" | awk '{print $NF}'
        fi
    elif [ -e "$path" ]; then
        # 如果不是符号链接但存在，返回绝对路径
        if command -v realpath >/dev/null 2>&1; then
            realpath "$path"
        else
            echo "$(cd "$(dirname "$path")" && pwd)/$(basename "$path")"
        fi
    else
        # 文件不存在，返回原路径
        echo "$path"
    fi
}

# 安全的目录创建和路径获取
get_absolute_path() {
    local path="$1"
    if [ -d "$path" ]; then
        if command -v realpath >/dev/null 2>&1; then
            realpath "$path"
        else
            cd "$path" && pwd
        fi
    else
        # 目录不存在，创建并返回路径
        mkdir -p "$path"
        if command -v realpath >/dev/null 2>&1; then
            realpath "$path"
        else
            echo "$(cd "$(dirname "$path")" && pwd)/$(basename "$path")"
        fi
    fi
}

if [ -f $DESKTOP_FILE ]; then
    OLD_UNINSTALL=$(dirname $(safe_readlink "$DESKTOP_FILE"))
    if [ -f $OLD_UNINSTALL/uninstall.sh ]; then
        #echo "Run $OLD_UNINSTALL/uninstall.sh"
        $OLD_UNINSTALL/uninstall.sh
    fi
fi

INSTALL_DIR=$(safe_readlink "$INSTALL_DIR")
if [ ! -d $INSTALL_DIR ]; then
    mkdir -p $INSTALL_DIR
    CREATE_INSTALL_DIR=1
fi

ROOT_DIR=$(dirname $(readlink -f $0))
if [ ! -d $DESKTOP_FILE_DIR ]; then
    mkdir -p $DESKTOP_FILE_DIR
fi

pushd $ROOT_DIR > /dev/null 

APPIMAGE_FILE=`ls RabbitIm_*.AppImage`
if [ $INSTALL_DIR != $ROOT_DIR ]; then
    cp $APPIMAGE_FILE $INSTALL_DIR/$APPIMAGE_FILE
    cp $APP_ID.png $INSTALL_DIR/$APP_ID.png
    cp $APP_ID.desktop $INSTALL_DIR/$APP_ID.desktop
fi

# 修改执行文件
sed -i "s#Exec=.*#Exec=$INSTALL_DIR/${APPIMAGE_FILE}#g" $INSTALL_DIR/$APP_ID.desktop
# 修改路径
sed -i "s#Path=.*#Path=${INSTALL_DIR}#g" $INSTALL_DIR/$APP_ID.desktop
if [ ! -f $DESKTOP_FILE ]; then
    CREATE_DESKTOP_FILE=1
    ln -s ${INSTALL_DIR}/$APP_ID.desktop $DESKTOP_FILE
    # ICON 使用绝对路径
    sed -i "s#^Icon=.*#Icon=$INSTALL_DIR/$APP_ID.png#" $INSTALL_DIR/$APP_ID.desktop
fi
# 修改执行权限
chmod a+xr $INSTALL_DIR/$APP_ID.desktop

echo "echo \"Uninstall \\\"Rabbit Instant Messaging\\\" AppImage from \\\"$(dirname $(readlink -f $DESKTOP_FILE))\\\"\"" > $INSTALL_DIR/uninstall.sh
if [ -n $CREATE_DESKTOP_FILE ]; then
    echo "rm $DESKTOP_FILE" >> $INSTALL_DIR/uninstall.sh
fi
if [ -n $CREATE_INSTALL_DIR ]; then
    echo "rm -fr $INSTALL_DIR" >> $INSTALL_DIR/uninstall.sh
fi
chmod u+x $INSTALL_DIR/uninstall.sh
chmod u+x $INSTALL_DIR/$APPIMAGE_FILE

echo ""
echo "Install \"Rabbit Instant Messaging\" AppImage to \"$INSTALL_DIR\"."
echo ""
echo "If you want to uninstall it. Please execute:"
echo "    $INSTALL_DIR/uninstall.sh"
echo ""

popd > /dev/null
