#!/bin/bash
set -ev

SOURCE_DIR=`pwd`
if [ -n "$1" ]; then
    SOURCE_DIR=$1
fi
TOOLS_DIR=${SOURCE_DIR}/Tools
ThirdLibs_DIR=${TOOLS_DIR}/ThirdLibs
cd ${SOURCE_DIR}
export RabbitCommon_DIR="${SOURCE_DIR}/RabbitCommon"
export PKG_CONFIG_PATH=${ThirdLibs_DIR}/lib/pkgconfig:${PKG_CONFIG_PATH}
if [ -f ${ThirdLibs_DIR}/change_prefix.sh ]; then
    cd ${ThirdLibs_DIR}
    ./change_prefix.sh
fi

if [ -z "${ENABLE_DOWNLOAD}" ]; then
    export ENABLE_DOWNLOAD=ON
fi

function version_gt() { test "$(echo "$@" | tr " " "\n" | sort -V | head -n 1)" != "$1"; }
function version_le() { test "$(echo "$@" | tr " " "\n" | sort -V | head -n 1)" == "$1"; }
function version_lt() { test "$(echo "$@" | tr " " "\n" | sort -rV | head -n 1)" != "$1"; }
function version_ge() { test "$(echo "$@" | tr " " "\n" | sort -rV | head -n 1)" == "$1"; }

cd ${SOURCE_DIR}

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${TOOLS_DIR}/android-sdk
    export ANDROID_NDK_ROOT=${TOOLS_DIR}/android-ndk
    #if [ -n "$APPVEYOR" ]; then
        #export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    #    if [ -z "${NDK_VERSION} " ]; then
    #        NDK_VERSION=20.0.5594570
    #    fi
    #    export ANDROID_NDK_ROOT=${TOOLS_DIR}/android-sdk/ndk-bundle
    #fi
    #if [ "$TRAVIS" = "true" ]; then
        #export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
    #fi
    export JAVA_HOME=${TOOLS_DIR}/android-studio/jre
    
    if version_ge $QT_VERSION_DIR 5.14 ; then
        export QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android
    else
        case $BUILD_ARCH in
            armeabi|armeabi-v7a)
                QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android_armv7
                if [ -z "${ANDROID_ARM_NEON}" ]; then
                    ANDROID_ARM_NEON=OFF
                fi
                ;;
            "armeabi-v7a with NEON")
                QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android_armv7
                ANDROID_ARM_NEON=ON
                ;;
            arm64-v8a)
                QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android_arm64_v8a/
                ;;
            x86)
                QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android_x86
                ;;
            x86_64)
                QT_ROOT=${TOOLS_DIR}/Qt/${QT_VERSION}/${QT_VERSION}/android_x86_64
                ;;
        esac
    fi
    export PATH=${TOOLS_DIR}/apache-ant/bin:$JAVA_HOME/bin:$PATH
    export ANDROID_SDK=${ANDROID_SDK_ROOT}
    export ANDROID_NDK=${ANDROID_NDK_ROOT}
    if [ -z "${BUILD_TOOS_VERSION}" ]; then
        export BUILD_TOOS_VERSION="28.0.3"
    fi
fi

if [ "${BUILD_TARGERT}" = "unix" ]; then
    if [ "$DOWNLOAD_QT" = "APT" ]; then
        export QT_ROOT=/usr/lib/`uname -m`-linux-gnu/qt5
    elif [ "$DOWNLOAD_QT" = "TRUE" ]; then
        QT_DIR=${TOOLS_DIR}/Qt/${QT_VERSION}
        export QT_ROOT=${QT_DIR}/${QT_VERSION}/gcc_64
    else
        #source /opt/qt${QT_VERSION_DIR}/bin/qt${QT_VERSION_DIR}-env.sh
        export QT_ROOT=/opt/qt${QT_VERSION_DIR}
    fi
    export PATH=$QT_ROOT/bin:$PATH
    export LD_LIBRARY_PATH=$QT_ROOT/lib/i386-linux-gnu:$QT_ROOT/lib:$LD_LIBRARY_PATH
    #export PKG_CONFIG_PATH=$QT_ROOT/lib/pkgconfig:$PKG_CONFIG_PATH
fi

if [ "$BUILD_TARGERT" != "windows_msvc" ]; then
    RABBIT_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
    if [ "$RABBIT_MAKE_JOB_PARA" = "-j1" ];then
        RABBIT_MAKE_JOB_PARA=""
    fi
fi

if [ "$BUILD_TARGERT" = "windows_mingw" \
    -a -n "$APPVEYOR" ]; then
    export PATH=/C/Qt/Tools/mingw${TOOLCHAIN_VERSION}/bin:$PATH
fi
TARGET_OS=`uname -s`
case $TARGET_OS in
    MINGW* | CYGWIN* | MSYS*)
        export PKG_CONFIG=/c/msys64/mingw32/bin/pkg-config.exe
        RABBIT_BUILD_HOST="windows"
        if [ "$BUILD_TARGERT" = "android" ]; then
            ANDROID_NDK_HOST=windows-x86_64
            if [ ! -d $ANDROID_NDK/prebuilt/${ANDROID_NDK_HOST} ]; then
                ANDROID_NDK_HOST=windows
            fi
            CONFIG_PARA="${CONFIG_PARA} -DCMAKE_MAKE_PROGRAM=make" #${ANDROID_NDK}/prebuilt/${ANDROID_NDK_HOST}/bin/make.exe"
        fi
        ;;
    Linux* | Unix*)
    ;;
    *)
    ;;
esac

export PATH=${QT_ROOT}/bin:$PATH
echo "PATH:$PATH"
echo "PKG_CONFIG:$PKG_CONFIG"
cd ${SOURCE_DIR}

case ${BUILD_TARGERT} in
    windows_msvc)
        MAKE=nmake
        ;;
    windows_mingw)
        if [ "${RABBIT_BUILD_HOST}"="windows" ]; then
            MAKE="mingw32-make ${RABBIT_MAKE_JOB_PARA}"
        fi
        ;;
    *)
        MAKE="make ${RABBIT_MAKE_JOB_PARA}"
        ;;
esac

if [ -n "$appveyor_build_version" -a -z "$VERSION" ]; then
    export VERSION="v0.0.4"
fi
if [ -z "$VERSION" ]; then
    export VERSION="v0.0.4"
fi
export UPLOADTOOL_BODY="Release RabbitIm ${VERSION}.<br/> The change see [ChangeLog.md](ChangeLog.md)"
#export UPLOADTOOL_PR_BODY=
        
if [ "${BUILD_TARGERT}" = "unix" ]; then
    cd $SOURCE_DIR
    if [ "${DOWNLOAD_QT}" != "TRUE" -a "${DOWNLOAD_QT}" != "APT" ]; then
        sed -i "s/export QT_VERSION_DIR=.*/export QT_VERSION_DIR=${QT_VERSION_DIR}/g" ${SOURCE_DIR}/debian/postinst
        sed -i "s/export QT_VERSION=.*/export QT_VERSION=${QT_VERSION}/g" ${SOURCE_DIR}/debian/preinst
        cat ${SOURCE_DIR}/debian/postinst
        cat ${SOURCE_DIR}/debian/preinst
    fi
    
    bash build_debpackage.sh ${QT_ROOT} ${ThirdLibs_DIR} ${RabbitCommon_DIR} ON

    sudo dpkg -i ../rabbitim_*_amd64.deb
    bash $SOURCE_DIR/test/test_linux.sh 

    cd debian/rabbitim/opt
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${QT_ROOT}/bin:${QT_ROOT}/lib:`pwd`/RabbitIm/bin:`pwd`/RabbitIm/lib
    wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage" -O linuxdeployqt.AppImage
    chmod a+x linuxdeployqt.AppImage

    cd RabbitIm
    ../linuxdeployqt.AppImage share/applications/*.desktop \
        -qmake=${QT_ROOT}/bin/qmake -appimage -no-copy-copyright-files -verbose

    # Create appimage install package
    cp $SOURCE_DIR/Install/install.sh .
    ln -s RabbitIm-${VERSION}-x86_64.AppImage RabbitIm-x86_64.AppImage
    tar -czf RabbitIm${VERSION}.tar.gz \
    RabbitIm-x86_64.AppImage \
    RabbitIm-${VERSION}-x86_64.AppImage \
    install.sh share
    
    MD5=`md5sum $SOURCE_DIR/../rabbitim_*_amd64.deb|awk '{print $1}'`
    echo "MD5:${MD5}"
    ./RabbitImApp \
        -f "`pwd`/update_linux.xml" \
        --md5 ${MD5}
    
    if [ "$TRAVIS_TAG" != "" -a "${QT_VERSION_DIR}" = "512" ]; then
        wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
        chmod u+x upload.sh
        ./upload.sh $SOURCE_DIR/../rabbitim_*_amd64.deb 
        ./upload.sh update_linux.xml 
        ./upload.sh RabbitIm_${VERSION}.tar.gz 
    fi
    exit 0
fi

if [ -n "$GENERATORS" ]; then
    if [ -n "${STATIC}" ]; then
        CONFIG_PARA="-DBUILD_SHARED_LIBS=${STATIC}"
    fi
    cmake -G"${GENERATORS}" ${SOURCE_DIR} ${CONFIG_PARA} \
         -DCMAKE_INSTALL_PREFIX=`pwd`/install \
         -DCMAKE_VERBOSE=ON \
         -DCMAKE_BUILD_TYPE=Release \
         -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5
    cmake --build . --target install --config Release -- ${RABBIT_MAKE_JOB_PARA}
else
    if [ "ON" = "${STATIC}" ]; then
        CONFIG_PARA="CONFIG*=static"
    fi
    if [ "${BUILD_TARGERT}" = "android" ]; then
        ${QT_ROOT}/bin/qmake ${SOURCE_DIR} \
            "CONFIG+=release" ${CONFIG_PARA}

        $MAKE
        $MAKE install INSTALL_ROOT=`pwd`/android-build
        ${QT_ROOT}/bin/androiddeployqt \
                       --input `pwd`/App/android-libRabbitImApp.so-deployment-settings.json \
                       --output `pwd`/android-build \
                       --android-platform ${ANDROID_API} \
                       --gradle --verbose
                       # --jdk ${JAVA_HOME}
        if [ "$TRAVIS_TAG" != "" -a "$BUILD_ARCH"="armeabi-v7a" -a "$QT_VERSION_DIR"="5.12" ]; then
        
		cp $SOURCE_DIR/Update/update_android.xml .
		    APK_FILE=`find . -name "android-build-debug.apk"`
		MD5=`md5sum ${APK_FILE} | awk '{print $1}'`
		echo "MD5:${MD5}"
		sed -i "s/<VERSION>.*</<VERSION>${VERSION}</g" update_android.xml
		sed -i "s/<INFO>.*</<INFO>Release Tasks-${VERSION}</g" update_android.xml
		sed -i "s/<TIME>.*</<TIME>`date`</g" update_android.xml
		sed -i "s/<ARCHITECTURE>.*</<ARCHITECTURE>${BUILD_ARCH}</g" update_android.xml
		sed -i "s/<MD5SUM>.*</<MD5SUM>${MD5}</g" update_android.xml
		sed -i "s:<URL>.*<:<URL>https\://github.com/KangLin/Tasks/releases/download/${VERSION}/android-build-debug.apk<:g" update_android.xml

		
		export UPLOADTOOL_BODY="Release RabbitIm-${VERSION}"
		#export UPLOADTOOL_PR_BODY=
		wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
		chmod u+x upload.sh
		./upload.sh ${APK_FILE} 
		./upload.sh update_android.xml
        fi
    else
        ${QT_ROOT}/bin/qmake ${SOURCE_DIR} \
                "CONFIG+=release" ${CONFIG_PARA}\
                PREFIX=`pwd`/install
                
        $MAKE
        echo "$MAKE install ...."
        $MAKE install
    fi
fi

if [ "${BUILD_TARGERT}" = "windows_msvc" ]; then
    if [ "${BUILD_ARCH}" = "x86" ]; then
        cp /C/OpenSSL-Win32/bin/libeay32.dll install/bin
        cp /C/OpenSSL-Win32/bin/ssleay32.dll install/bin
    elif [ "${BUILD_ARCH}" = "x64" ]; then
        cp /C/OpenSSL-Win64/bin/libeay32.dll install/bin
        cp /C/OpenSSL-Win64/bin/ssleay32.dll install/bin
    fi
    
    if [ -z "${STATIC}" ]; then
        "/C/Program Files (x86)/NSIS/makensis.exe" "Install.nsi"
        MD5=`md5sum Tasks-Setup-*.exe|awk '{print $1}'`
        echo "MD5:${MD5}"
        install/bin/TasksApp.exe -f "`pwd`/update_windows.xml" --md5 ${MD5}
        
        cat update_windows.xml
    fi
fi
