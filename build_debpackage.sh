#!/bin/bash

if [ -n "$1" -a -z "$QT_ROOT" ]; then
	QT_ROOT=$1
fi

if [ ! -f /usr/bin/qmake -a -z "$QT_ROOT" ]; then
	echo "./$1 QT_ROOT RabbitCommon_DIR"
    exit -1
fi

if [ -n "$2" -a -z "$RabbitCommon_DIR" ]; then
	RabbitCommon_DIR=$2
fi

if [ -z "$RabbitCommon_DIR" ]; then
	RabbitCommon_DIR=`pwd`/../RabbitCommon
fi

if [ ! -d "$RabbitCommon_DIR" ]; then
	echo "./$1 QT_ROOT RabbitCommon_DIR"
        exit -2
fi

export RabbitCommon_DIR=$RabbitCommon_DIR
export QT_ROOT=$QT_ROOT
export PATH=$QT_ROOT/bin:$PATH
export LD_LIBRARY_PATH=$QT_ROOT/lib/i386-linux-gnu:$QT_ROOT/lib:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$QT_ROOT/lib/pkgconfig:$PKG_CONFIG_PATH
#fakeroot debian/rules binary 
dpkg-buildpackage -us -uc -b

