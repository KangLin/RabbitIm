#!/bin/sh
set -ev
echo "build-and-test pwd:`pwd`"

QMAKE_ARGS="QXMPP_USE_VPX=1 RABBITIM_USER_FFMPEG=1 RABBITIM_USER_LIBCURL=1 RABBITIM_USER_OPENSSL=1"

if [ -n "$CC" ]; then
    QMAKE_ARGS="$QMAKE_ARGS QMAKE_CC=$CC"
fi

if [ -n "$CXX" ]; then
    QMAKE_ARGS="$QMAKE_ARGS QMAKE_CXX=$CXX"
fi

# compile
`pwd`/ThirdLibary/build_script/qt/bin/qmake $QMAKE_ARGS
make VERBOSE=1
