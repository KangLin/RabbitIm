#!/bin/bash

function help()
{
    echo "$0 QT_ROOT THIRD_LIBRARY_PATH [RabbitCommon_DIR]"
}

if [ -n "$1" -a -z "$QT_ROOT" ]; then
    QT_ROOT=$1
fi

if [ ! -f /usr/bin/qmake -a -z "$QT_ROOT" ]; then
    help
    exit -1
fi

if [ -n "$2" -a -z "$THIRD_LIBRARY_PATH" ]; then
    export THIRD_LIBRARY_PATH=$2
    export LD_LIBRARY_PATH=$THIRD_LIBRARY_PATH/lib
    export PKG_CONFIG_PATH=$THIRD_LIBRARY_PATH/lib/pkgconfig
else
    help
    exit -2
fi

if [ -n "$3" -a -z "$RabbitCommon_DIR" ]; then
    RabbitCommon_DIR=$3
fi

if [ -z "$RabbitCommon_DIR" ]; then
    RabbitCommon_DIR=`pwd`/../RabbitCommon
fi

if [ ! -d "$RabbitCommon_DIR" ]; then
    help
    exit -3
fi

export RabbitCommon_DIR=$RabbitCommon_DIR
export QT_ROOT=$QT_ROOT
export PATH=$QT_ROOT/bin:$PATH
export LD_LIBRARY_PATH=$QT_ROOT/lib/i386-linux-gnu:$QT_ROOT/lib:`pwd`/debian/rabbitim/opt/RabbitIm/lib:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$QT_ROOT/lib/pkgconfig:$PKG_CONFIG_PATH
fakeroot debian/rules binary 
#dpkg-buildpackage -us -uc -b

