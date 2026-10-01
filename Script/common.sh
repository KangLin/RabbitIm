#!/bin/bash
# Author: Kang Lin <kl222@126.com>

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

REPO_ROOT=$(safe_readlink $(dirname $(dirname $(safe_readlink ${BASH_SOURCE[0]}))))
echo "REPO_ROOT: $REPO_ROOT"
if [ -z "$RabbitCommon_ROOT" ]; then
    if [ -f "$REPO_ROOT/../RabbitCommon/Script/RabbitCommon.sh" ]; then
        export RabbitCommon_ROOT=$REPO_ROOT/../RabbitCommon
    fi
fi

if [ -z "$RabbitCommon_ROOT" ]; then
    echo "RabbitCommon_ROOT is not found. Please use one of the following ways to set it:"
    echo "1. Set RabbitCommon_ROOT to source code root of RabbitCommon."
    echo "1.1 Please download the source code of RabbitCommon from https://github.com/KangLin/RabbitCommon"
    echo "   ag:"
    echo "       git clone https://github.com/KangLin/RabbitCommon.git $REPO_ROOT/.."
    echo "1.2 Then set environment variable RabbitCommon_ROOT to download root directory."
    echo "   ag:"
    echo "       export RabbitCommon_ROOT=$REPO_ROOT/../RabbitCommon "
    echo ""
    read -t 3 -p "Download RabbitCommon from https://github.com/KangLin/RabbitCommon?  (Y/n): " INPUT || true
    if [ "${INPUT:-Y}" != "Y" ] && [ "${INPUT:-Y}" != "y" ]; then
        echo "Deployment cancelled"
        exit 1
    fi
    echo "Starting download..."
    if ! command -v git &> /dev/null; then
        echo "Error: git is not installed"
        exit 1
    fi
    echo "git clone https://github.com/KangLin/RabbitCommon.git $REPO_ROOT/../RabbitCommon"
    git clone https://github.com/KangLin/RabbitCommon.git $REPO_ROOT/../RabbitCommon
    if [ $? -ne 0 ]; then
        echo "Failed to clone RabbitCommon"
        exit 1
    fi
    export RabbitCommon_ROOT=$REPO_ROOT/../RabbitCommon
fi
if [ ! -f "$RabbitCommon_ROOT/Script/RabbitCommon.sh" ]; then
    echo "Failed to set 'RabbitCommon_ROOT=$RabbitCommon_ROOT' to the RabbitCommon root, please set it correctly"
    exit 1
fi

source $RabbitCommon_ROOT/Script/RabbitCommon.sh
init_global
