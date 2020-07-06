#!/bin/bash 
#下载工具  

set -ev

SOURCE_DIR="`pwd`"
echo $SOURCE_DIR
TOOLS_DIR=${SOURCE_DIR}/Tools
PACKAGE_DIR=${SOURCE_DIR}/Package
ThirdLibs_DIR=${TOOLS_DIR}/ThirdLibs
echo ${TOOLS_DIR}

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${TOOLS_DIR}/android-sdk
    export ANDROID_NDK_ROOT=${TOOLS_DIR}/android-ndk
    export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    export PATH=${TOOLS_DIR}/apache-ant/bin:$JAVA_HOME:$PATH
else
    exit 0
fi

if [ ! -d "${TOOLS_DIR}" ]; then
    mkdir ${TOOLS_DIR}
fi
if [ ! -d "$PACKAGE_DIR" ]; then
    mkdir -p $PACKAGE_DIR
fi
cd ${TOOLS_DIR}

# Qt qt安装参见：https://github.com/benlau/qtci  
if [ -n "${QT_VERSION}" ]; then
    QT_DIR=C:/projects/${APPVEYOR_PROJECT_NAME}/Tools/Qt/${QT_VERSION}
    if [ ! -d "${QT_DIR}" ]; then
        wget -c --no-check-certificate -nv http://download.qt.io/official_releases/qt/${QT_VERSION_DIR}/${QT_VERSION}/qt-opensource-windows-x86-${QT_VERSION}.exe
        bash ${SOURCE_DIR}/ci/qt-installer.sh qt-opensource-windows-x86-${QT_VERSION}.exe ${QT_DIR}
        rm qt-opensource-windows-x86-${QT_VERSION}.exe
    fi
fi

# Download third libraries
if [ -n "$DOWNLOAD_THIRDLIBS_URL" ]; then
    if [ ! -d ${ThirdLibs_DIR} ]; then
        mkdir -p ${ThirdLibs_DIR}
    fi
    cd ${ThirdLibs_DIR}
    ThirdLibs_File=third_libs.tar.gz
    wget -c --no-check-certificate $DOWNLOAD_THIRDLIBS_URL -O $ThirdLibs_File
    tar xzvf $ThirdLibs_File
fi

cd ${TOOLS_DIR}

#下载ANT 
#wget -c -nv http://apache.fayea.com//ant/binaries/apache-ant-1.10.1-bin.tar.gz
#tar xzf apache-ant-1.10.1-bin.tar.gz
#rm -f apache-ant-1.10.1-bin.tar.gz
#mv apache-ant-1.10.1 apache-ant

cd ${TOOLS_DIR}

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
    unzip -q ${NDK_PACKAGE} -d ${TOOLS_DIR}
    cd ${TOOLS_DIR}
    mv android-ndk-r21 android-ndk
}

cd ${SOURCE_DIR}

install_android_sdk_and_ndk
