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
QT_CLEAN="clean"
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
    RABBITIM_BUILD_SOURCE_CODE=${RABBITIM_BUILD_PREFIX}/../src/qt5
fi

CUR_DIR=`pwd`

#下载源码:
if [ ! -d ${RABBITIM_BUILD_SOURCE_CODE} ]; then
    echo "git clone https://git.gitorious.org/qt/qt5.git ${RABBITIM_BUILD_SOURCE_CODE}"
    git clone https://git.gitorious.org/qt/qt5.git ${RABBITIM_BUILD_SOURCE_CODE}
    cd ${RABBITIM_BUILD_SOURCE_CODE}
    perl init-repository -no-qtcanvas3d -no-qt3d
    cd ${CUR_DIR}
fi

cd ${RABBITIM_BUILD_SOURCE_CODE}

#清理
if [ -n "$QT_CLEAN" ]; then
	git clean -xdf
	git submodule foreach --recursive "git clean -dfx"
	echo $1
	for i in `ls $1`;
	do
        	if [ -d $1/${i} ]; then
                	echo "$1/${i}"
	                cd $1/${i}
        	        git clean -xdf
	        fi
	done
	rm -fr ${RABBITIM_BUILD_PREFIX}/qt
fi

echo "RABBITIM_BUILD_SOURCE_CODE:$RABBITIM_BUILD_SOURCE_CODE"
echo "CUR_DIR:`pwd`"
echo "RABBITIM_BUILD_PREFIX:$RABBITIM_BUILD_PREFIX"
echo "RABBITIM_BUILD_HOST:$RABBITIM_BUILD_HOST"
echo "RABBITIM_BUILD_CROSS_HOST:$RABBITIM_BUILD_CROSS_HOST"
echo "RABBITIM_BUILD_CROSS_PREFIX:$RABBITIM_BUILD_CROSS_PREFIX"
echo "RABBITIM_BUILD_CROSS_SYSROOT:$RABBITIM_BUILD_CROSS_SYSROOT"
echo ""

echo "configure ..."

CONFIG_PARA="-opensource -confirm-license -nomake examples -nomake tests -no-compile-examples"
CONFIG_PARA="${CONFIG_PARA} -no-sql-sqlite -no-sql-odbc"
CONFIG_PARA="${CONFIG_PARA} -skip qtdoc -skip qtcanvas3d -skip qtwebkit-examples -skip qt3d"
CONFIG_PARA="${CONFIG_PARA} -prefix ${RABBITIM_BUILD_PREFIX}/qt"
CONFIG_PARA="${CONFIG_PARA} -I ${RABBITIM_BUILD_PREFIX}/include -L ${RABBITIM_BUILD_PREFIX}/lib"

case ${RABBITIM_BUILD_TARGERT} in
    android)
		export PKG_CONFIG_SYSROOT_DIR=${RABBITIM_BUILD_PREFIX} #qt编译时需要
		export PKG_CONFIG_LIBDIR=${RABBITIM_BUILD_PREFIX}/lib/pkgconfig
        CONFIG_PARA="${CONFIG_PARA} -xplatform android-g++ -android-ndk ${ANDROID_NDK_ROOT}"
        CONFIG_PARA="${CONFIG_PARA} -android-sdk ${ANDROID_SDK_ROOT}"
        CONFIG_PARA="${CONFIG_PARA} -android-ndk-host ${RABBITIM_BUILD_HOST}"
        CONFIG_PARA="${CONFIG_PARA} -android-toolchain-version ${RABBITIM_BUILD_TOOLCHAIN_VERSION}"
        CONFIG_PARA="${CONFIG_PARA} -android-ndk-platform android-${RABBITIM_BUILD_PLATFORMS_VERSION}"
        MODULE_PARA="${MODULE_PARA} module-qtandroidextras"
    	;;
    unix)
		CONFIG_PARA="${CONFIG_PARA} -skip qtandroidextras -skip qtandroidextras -skip qtmacextras -skip qtwinextras"
		;;
    windows_msvc)
    	;;
    windows_mingw)
    	;;
	unix_mingw)
		export PKG_CONFIG_SYSROOT_DIR=${RABBITIM_BUILD_PREFIX} #qt编译时需要
		export PKG_CONFIG_LIBDIR=${RABBITIM_BUILD_PREFIX}/lib/pkgconfig
		CONFIG_PARA="${CONFIG_PARA} -release -xplatform win32-g++"
		CONFIG_PARA="${CONFIG_PARA} -device-option CROSS_COMPILE=${RABBITIM_BUILD_CROSS_PREFIX}"
		CONFIG_PARA="${CONFIG_PARA} -skip qtandroidextras -skip qtx11extras -skip qtmacextras"
		CONFIG_PARA="${CONFIG_PARA} -skip qtwebkit"
		;;
    *)
		echo "${HELP_STRING}"
		return 2
		;;
esac

echo "./configure ${CONFIG_PARA}"
./configure ${CONFIG_PARA}

echo "make install"
make -j2 \
	&& make install \
	&& cp qtbase/bin/qt.conf ${RABBITIM_BUILD_PREFIX}/qt/bin/qt.conf

cd $CUR_DIR
