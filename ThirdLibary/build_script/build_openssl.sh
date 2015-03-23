#作者：康林
#参数:
#    $1:编译目标(android、windows_msvc、windows_mingw、unix、unix_mingw)
#    $2:源码的位置 

#运行本脚本前,先运行 build_$1_envsetup.sh 进行环境变量设置,需要先设置下面变量:
#   RABBITIM_BUILD_TARGERT   编译目标（android、windows_msvc、windows_mingw、unix、unix_mingw）
#   RABBITIM_BUILD_PREFIX=`pwd`/../${RABBITIM_BUILD_TARGERT}  #修改这里为安装前缀
#   RABBITIM_BUILD_SOURCE_CODE    #源码目录
#   RABBITIM_BUILD_CROSS_PREFIX     #交叉编译前缀
#   RABBITIM_BUILD_CROSS_SYSROOT  #交叉编译平台的 sysroot

set -e
HELP_STRING="Usage $0 PLATFORM (android|windows_msvc|windows_mingw|unix|unix_mingw) [SOURCE_CODE_ROOT_DIRECTORY]"

case $1 in
    android|windows_msvc|windows_mingw|unix|unix_mingw)
    RABBITIM_BUILD_TARGERT=$1
    ;;
    *)
    echo "${HELP_STRING}"
    return 1
    ;;
esac

if [ -z "${RABBITIM_BUILD_PREFIX}" ]; then
    echo "source build_${RABBITIM_BUILD_TARGERT}_envsetup.sh"
    source build_${RABBITIM_BUILD_TARGERT}_envsetup.sh || return 1
fi

if [ -n "$2" ]; then
    RABBITIM_BUILD_SOURCE_CODE=$2
else
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/openssl
fi

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    echo "git clone https://github.com/openssl/openssl  ${RABBITIM_BUILD_SOURCE_CODE}"
    git clone https://github.com/openssl/openssl ${RABBITIM_BUILD_SOURCE_CODE}
fi

CUR_DIR=`pwd`
cd ${RABBITIM_BUILD_SOURCE_CODE}

echo ""
echo "RABBITIM_BUILD_TARGERT:${RABBITIM_BUILD_TARGERT}"
echo "RABBITIM_BUILD_SOURCE_CODE:$RABBITIM_BUILD_SOURCE_CODE"
echo "CUR_DIR:`pwd`"
echo "RABBITIM_BUILD_PREFIX:$RABBITIM_BUILD_PREFIX"
echo "RABBITIM_BUILD_HOST:$RABBITIM_BUILD_HOST"
echo "RABBITIM_BUILD_CROSS_HOST:$RABBITIM_BUILD_CROSS_HOST"
echo "RABBITIM_BUILD_CROSS_PREFIX:$RABBITIM_BUILD_CROSS_PREFIX"
echo "RABBITIM_BUILD_CROSS_SYSROOT:$RABBITIM_BUILD_CROSS_SYSROOT"
echo ""

git clean -xdf
echo "configure ..."
case ${RABBITIM_BUILD_TARGERT} in
    android)
		export ANDROID_DEV="${RABBITIM_BUILD_CROSS_SYSROOT}/usr"
		perl Configure --cross-compile-prefix=${RABBITIM_BUILD_CROSS_PREFIX} \
				--prefix=${RABBITIM_BUILD_PREFIX} \
				--openssldir=${RABBITIM_BUILD_PREFIX} \
				android-armv7 #\
#				--sysroot="${RABBITIM_BUILD_CROSS_SYSROOT}"
		;;
    unix)
		./config --prefix=${RABBITIM_BUILD_PREFIX} --openssldir=${RABBITIM_BUILD_PREFIX} shared
		;;
    windows_msvc)
		;;
    windows_mingw)
		;;
	unix_mingw)
		perl Configure  --prefix=${RABBITIM_BUILD_PREFIX} \
			--openssldir=${RABBITIM_BUILD_PREFIX} \
			--cross-compile-prefix=${RABBITIM_BUILD_CROSS_PREFIX} \
			shared mingw
		;;
    *)
		echo "${HELP_STRING}"
		return 2
		;;
esac

echo "make install"
make  && make install

cd $CUR_DIR
