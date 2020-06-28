#!/bin/bash 
#下载工具  

# DOWNLOAD_THIRDLIBS_URL: 第三方库的下载地址
# DOWNLOAD_QT: TRUE 从 http://download.qt.io/official_releases/qt 下载QT
# QT_VERSION：Qt 版本号
# QT_VERSION_DIR：

set -ev

SOURCE_DIR="`pwd`"
echo "SOURCE_DIR:$SOURCE_DIR"
TOOLS_DIR=${SOURCE_DIR}/Tools
PACKAGE_DIR=${SOURCE_DIR}/Package
ThirdLibs_DIR=${TOOLS_DIR}/ThirdLibs
echo ${TOOLS_DIR}

if [ ! -d "${TOOLS_DIR}" ]; then
    mkdir ${TOOLS_DIR}
fi

if [ ! -d ${ThirdLibs_DIR} ]; then
    mkdir -p ${ThirdLibs_DIR}
fi

if [ ! -d ${PACKAGE_DIR} ]; then
    mkdir -p ${PACKAGE_DIR}
fi

function function_common()
{
    cd ${TOOLS_DIR}
    
    # Download third libraries
    if [ -n "$DOWNLOAD_THIRDLIBS_URL" ]; then
        cd ${ThirdLibs_DIR}
        echo "ThirdLibs_DIR:`pwd`"
        ThirdLibs_File=third_libs.zip
        wget -c -nv --no-check-certificate $DOWNLOAD_THIRDLIBS_URL -O $ThirdLibs_File
        unzip $ThirdLibs_File
    fi

    # Qt qt安装参见：https://github.com/benlau/qtci
    cd ${TOOLS_DIR}
    if [ "$DOWNLOAD_QT" = "TRUE" ]; then
        QT_DIR=C:/projects/${APPVEYOR_PROJECT_NAME}/Tools/Qt/${QT_VERSION}
        if [ ! -d "${QT_DIR}" ]; then
            #cd ${PACKAGE_DIR}
            wget -c --no-check-certificate -nv http://download.qt.io/official_releases/qt/${QT_VERSION_DIR}/${QT_VERSION}/qt-opensource-windows-x86-${QT_VERSION}.exe
            bash ${SOURCE_DIR}/ci/qt-installer.sh qt-opensource-windows-x86-${QT_VERSION}.exe ${QT_DIR}
            rm qt-opensource-windows-x86-${QT_VERSION}.exe
        fi
    fi
    
    cd ${SOURCE_DIR}
}

function function_windows_msvc()
{
    function_common
    cd ${SOURCE_DIR}
}

function install_android()
{
    cd ${TOOLS_DIR}

    if [ -n "$1" ]; then
        NDK="ndk-bundle"
    fi
    
    #Download android sdk  
    if [ ! -d "${TOOLS_DIR}/android-sdk" ]; then
        
        cd ${PACKAGE_DIR}
        ANDROID_STUDIO_VERSION=192.6241897
        if [ ! -f android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip ]; then
            wget -c -nv https://dl.google.com/dl/android/studio/ide-zips/3.6.1.0/android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip
        fi
        
        cd ${TOOLS_DIR}
        cp ${PACKAGE_DIR}/android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip .
        unzip -q android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip
        rm android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip
        export JAVA_HOME=${TOOLS_DIR}/android-studio/jre
        export PATH=${JAVA_HOME}/bin:$PATH

        SDK_VERSION=4333796
        cd ${PACKAGE_DIR}
        if [ ! -f sdk-tools-windows-${SDK_VERSION}.zip ]; then
            wget -c -nv https://dl.google.com/android/repository/sdk-tools-windows-${SDK_VERSION}.zip
        fi
        cd ${TOOLS_DIR}
        mkdir android-sdk
        cd android-sdk
        mv ${PACKAGE_DIR}/sdk-tools-windows-${SDK_VERSION}.zip .
        unzip -q sdk-tools-windows-${SDK_VERSION}.zip
        rm sdk-tools-windows-${SDK_VERSION}.zip

        echo "Install sdk and ndk ......"
        if [ -n "${ANDROID_API}" ]; then
            PLATFORMS="platforms;${ANDROID_API}"
        else
            PLATFORMS="platforms"
        fi
        if [ -z "${BUILD_TOOS_VERSION}" ]; then
            BUILD_TOOS_VERSION="28.0.3"
        fi
        (sleep 5 ; num=0 ; while [ $num -le 5 ] ; do sleep 1 ; num=$(($num+1)) ; printf 'y\r\n' ; done ) \
        | ./tools/bin/sdkmanager.bat "platform-tools" "build-tools;${BUILD_TOOS_VERSION}" "${PLATFORMS}" ${NDK}
        
        export ANDROID_SDK_ROOT=${TOOLS_DIR}/android-sdk
        if [ -n "${NDK}" ]; then
            mv ${TOOLS_DIR}/android-sdk/ndk-bundle ${TOOLS_DIR}/android-ndk
        fi
        cd ${TOOLS_DIR}
    fi
    
    cd ${SOURCE_DIR}
}

function install_android_sdk_and_ndk()
{
    install_android
    NDK_VERSION=r21
    NDK_PACKAGE=android-ndk-${NDK_VERSION}-windows-x86_64.zip
    cd ${PACKAGE_DIR}
    if [ ! -f ${NDK_PACKAGE} ]; then
        wget -c -nv https://dl.google.com/android/repository/${NDK_PACKAGE}
    fi
    echo "unzip -q ${NDK_PACKAGE} -d ${TOOLS_DIR}"
    unzip -q ${NDK_PACKAGE} -d ${TOOLS_DIR}
    cd ${TOOLS_DIR}
    mv android-ndk-r21 android-ndk
}

function function_android()
{
    install_android_sdk_and_ndk
    function_common

    cd ${TOOLS_DIR}
}

function function_mingw()
{
    function_common
    cd ${SOURCE_DIR}
}

case ${BUILD_TARGERT} in
    android)
        function_android
        ;;
    windows_msvc)
        function_windows_msvc
        ;;
    windows_mingw)
        function_mingw
        ;;
    *)
    echo "There aren't ${BUILD_TARGERT}"
        ;;
esac

cd ${SOURCE_DIR}
