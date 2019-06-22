#!/bin/bash
set -ev

SOURCE_DIR=`pwd`
if [ -n "$1" ]; then
    SOURCE_DIR=$1
fi

cd ${SOURCE_DIR}

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${SOURCE_DIR}/Tools/android-sdk
    export ANDROID_NDK_ROOT=${SOURCE_DIR}/Tools/android-ndk
    if [ -n "$APPVEYOR" ]; then
        export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    fi
    if [ "$TRAVIS" = "true" ]; then
        export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
    fi
    case $BUILD_ARCH in
        arm*)
            export QT_ROOT=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}/${QT_VERSION}/android_armv7
            ;;
        x86)
        export QT_ROOT=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}/${QT_VERSION}/android_x86
        ;;
    esac
    export PATH=${SOURCE_DIR}/Tools/apache-ant/bin:$JAVA_HOME:$PATH
fi

if [ "${BUILD_TARGERT}" = "unix" ]; then
    if [ "$BUILD_DOWNLOAD" = "TRUE" ]; then
        QT_DIR=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}
        export QT_ROOT=${QT_DIR}/${QT_VERSION}/gcc_64
    else
        #source /opt/qt${QT_VERSION_DIR}/bin/qt${QT_VERSION_DIR}-env.sh
        export QT_ROOT=/opt/qt${QT_VERSION_DIR}
    fi
    export PATH=$QT_ROOT/bin:$PATH
    export LD_LIBRARY_PATH=$QT_ROOT/lib/i386-linux-gnu:$QT_ROOT/lib:$LD_LIBRARY_PATH
    export PKG_CONFIG_PATH=$QT_ROOT/lib/pkgconfig:$PKG_CONFIG_PATH
fi

if [ "$BUILD_TARGERT" != "windows_msvc" ]; then
    RABBIT_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
    if [ "$RABBIT_MAKE_JOB_PARA" = "-j1" ];then
        RABBIT_MAKE_JOB_PARA="-j2"
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
if [ "${BUILD_TARGERT}" = "windows_msvc" ]; then
    ./tag.sh
fi
mkdir -p build_${BUILD_TARGERT}
cd build_${BUILD_TARGERT}

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

export VERSION="0.0.10"
if [ "${BUILD_TARGERT}" = "unix" ]; then
    cd $SOURCE_DIR
    bash build_debpackage.sh ${QT_ROOT}

    sudo dpkg -i ../tasks_${VERSION}_amd64.deb
    $SOURCE_DIR/test/test_linux.sh 
    
    if [ "$TRAVIS_TAG" != "" -a "${QT_VERSION_DIR}" = "512" ]; then
        cd debian/tasks/opt/Tasks
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${QT_ROOT}/bin:${QT_ROOT}/lib:`pwd`/debian/tasks/opt/Tasks/bin:`pwd`/debian/tasks/opt/Tasks/lib
        wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
        chmod a+x linuxdeployqt-continuous-x86_64.AppImage
        
        ./linuxdeployqt-continuous-x86_64.AppImage share/applications/*.desktop \
                -qmake=${QT_ROOT}/bin/qmake -appimage
    
        # Create appimage install package
        cp $SOURCE_DIR/Install/install.sh .
        ln -s Tasks-${VERSION}-x86_64.AppImage Tasks-x86_64.AppImage
        tar -czf Tasks_${VERSION}.tar.gz \
            Tasks-x86_64.AppImage \
            Tasks-${VERSION}-x86_64.AppImage \
            install.sh share
    
        MD5=`md5sum $SOURCE_DIR/../tasks_${VERSION}_amd64.deb|awk '{print $1}'`
        echo "MD5:${MD5}"
        ./bin/TasksApp \
            -f "`pwd`/update_linux.xml" \
            --md5 ${MD5}
        export UPLOADTOOL_BODY="Release Tasks-${VERSION}"
        #export UPLOADTOOL_PR_BODY=
        wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
        chmod u+x upload.sh
        ./upload.sh $SOURCE_DIR/../tasks_${VERSION}_amd64.deb 
        ./upload.sh update_linux.xml 
        ./upload.sh Tasks_${VERSION}.tar.gz 
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
                       --input `pwd`/App/android-libTasksApp.so-deployment-settings.json \
                       --output `pwd`/android-build \
                       --android-platform ${ANDROID_API} \
                       --gradle --verbose
                       # --jdk ${JAVA_HOME}
        
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

        if [ "$TRAVIS_TAG" != "" ]; then
                export UPLOADTOOL_BODY="Release Tasks-${VERSION}"
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
