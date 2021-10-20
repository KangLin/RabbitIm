### 目录

- [工具](#user-content-工具)
- [第三方库](#user-content-第三方库)
- [本项目](#user-content-本项目)
- [编译](#user-content-编译)  
    各目标编译详细说明：
    + [ubuntu](https://github.com/KangLin/RabbitThirdLibrary/blob/master/INSTALL_UBUNTU.md)
    + [android](https://github.com/KangLin/RabbitThirdLibrary/blob/master/INSTALL_ANDROID.md)
    + [windows](https://github.com/KangLin/RabbitThirdLibrary/blob/master/INSTALL_WINDOWS.md)
- [本工程编译](#user-content-本工程编译)
- [翻译文件部署](#user-content-翻译文件部署)
- [调试](#user-content-调试)

### 工具

#### bash 环境:
    
- windows：
  * msys2: http://sourceforge.net/projects/msys2/

    ```
    wget https://sourceforge.net/projects/msys2/files/latest/download
    ```

    + 代码库: https://github.com/Alexpux/MSYS2-packages
  * msys: http://www.mingw.org/wiki/MSYS
  * cygwin主页: http://www.cygwin.org/  
  本人使用 msys2 
  
#### 下载工具：
- wget
- curl

#### 脚本工具：
* python:2.7.6  版本 2.7 或以后
  + 主页: https://www.python.org/  
               http://www.activestate.com/activepython/
* perl：5.18.2  版本 5.12 或以后
  + 主页： http://www.perl.org/  
                http://www.activestate.com/activeperl/
* bash:msys或cygwin
* ruby:qtwebkit编译需要 版本 1.9.3 或以后
  + 主页：
    * http://www.ruby-lang.org/
    * http://rubyinstaller.org/  
    * https://github.com/ruby/ruby
#### cmake：
* 主页：http://www.cmake.org/
* 当前使用版本：3.1 
#### 安装auto工具：
    
1. automake: http://www.gnu.org/software/automake/
2. autoconf: http://www.gnu.org/software/autoconf/
3. libtool: http://www.gnu.org/software/libtool/
4. m4: http://www.gnu.org/software/m4
5. pkg-config: http://www.freedesktop.org/wiki/Software/pkg-config/
6. make:根据bash系统的不同，有msys make（msys bash）、mingw make、gnu make（cygwin bash）

#### 版本控制工具：

- subversion: http://subversion.apache.org/
- git: http://www.git-scm.com/
  + [git设置](http://blog.csdn.net/kl222/article/details/32903495)
  
#### windows平台工具：
- Visual Studio
  * 主页
    - https://visualstudio.microsoft.com/vs/
    - http://msdn.microsoft.com/zh-cn/vstudio 
    - https://visualstudio.microsoft.com/zh-hans/downloads/
  * 版本：
    - Visual Studio 2013
    - Visual Studio 2015
    - Visual Studio 2017
    - Visual Studio 2019
  * Visual Studio 各版本密钥：https://blog.csdn.net/kl222/article/details/84939135
  * 当前使用版本：vs 2017
- windows sdk: https://developer.microsoft.com/en-us/windows/downloads/sdk-archive  
    根据你的操作系统安装相应的 SDK。 CDB必须要安装，QT调试需要。
- Windows Driver Kit: https://docs.microsoft.com/zh-cn/windows-hardware/drivers/download-the-wdk

#### 汇编工具（yasm、nasm）：
- [yasm](http://yasm.tortall.net/)
- [nasm](http://www.nasm.us/)  
    下载并安装，并且设置路径到环境变量PATH
    
#### 编译工具：
- windows
  + msvc
  + mingw gcc （4.82、4.91、4.92、5.30、5.40）
- linux
  + gcc、g++
          当前使用版本：4.8
- android
  + android ndk
  + android sdk
  
#### android工具：
    1. Java jdk：http://www.oracle.com/technetwork/java/javase/downloads/index.html
    2. ant：http://ant.apache.org/
    3. android sdk：http://developer.android.com/sdk/index.html
    4. android ndk：http://developer.android.com/ndk/index.html
    
#### 安装工具
    1. deb打包工具：dh_make、 debhelper
    2. rpm打包工具:
    3. windows安装工具:nsis
#### icu工具，编译 QtWebkit 需要
#### [Qt工具与版本](http://wiki.qt.io/Qt-5.5.0-tools-and-versions)
#### QT：
    * 主页：https://www.qt.io
    * 下载：http://download.qt.io/official_releases/qt/
    * 当前使用版本：5.7.0
        + QT开发工具参考：http://doc.qt.io/qt-4.8/developing-with-qt.html
        + [QT编译](http://blog.csdn.net/kl222/article/details/44216951)

### 第三方库

1. [可选] XMPP协议（QXMPP）：
    * 主页：http://code.google.com/p/qxmpp
    * 下载：

        git clone https://github.com/KangLin/qxmpp.git

    * 当前使用版本：  
        git库版本 —— v1.0.0  
      编译方法参考源码根目录下的README文件。
2. [可选] 音视频框架
    1. webrtc库：
        * 主页：http://www.webrtc.org/  
        * 下载：

            gclient config http://webrtc.googlecode.com/svn/trunk  
            gclient sync --force  

        * 当前使用版本：tag ——r8464
        * 编译参考：http://blog.csdn.net/kl222/article/details/17198873
    2. [ffmpeg](http://www.ffmpeg.org/)
        * 下载：

        git clone git://source.ffmpeg.org/ffmpeg.git

        *当前使用版本：
         git库版本 —— n4.0.4
3. [可选] 视频编解码器
    1. x264
        * 下载：`git clone git://git.videolan.org/x264.git`
        * 当前使用版本：  
          git库版本 —— 5c65704
    2. 解码库(libvpx)
        * 下载：`git clone https://chromium.googlesource.com/webm/libvpx libvpx`  
        * 当前使用版本:  
          git库版本 —— v1.8.0
    3. libyuv
        * 下载:`git clone http://git.chromium.org/external/libyuv.git`
        * 当前使用版本:  
        git库版本 —— 416c48d
4. [可选] 音频编解码器
    1. 音频处理库(libspeexdsp)
        * 下载：`git clone http://git.xiph.org/speexdsp.git`
        * 当前使用版本:  
        git库版本 —— 887ac10
    2. 音频编解码库（libspeex）
        * 下载：`git clone http://git.xiph.org/speex.git`  
        * 当前使用版本：  
        git库版本 —— 6aab25c
    3. opus
        * 下载： `git clone git://git.opus-codec.org/opus.git`
        * 当前使用版本：1.3.1
5. [可选] [图像处理库(opencv)](http://opencv.org/)
    * 下载：`git clone git://github.com/Itseez/opencv.git`  
    * 当前使用版本:  
    git库版本 —— 2.4.11
6. [可选] [openssl](http://www.openssl.org/)
    * 下载：`git clone  https://github.com/openssl/openssl`  
    * 当前使用版本：  
    git库版本 —— OpenSSL_1_1_1b
7. [可选] libsodium
    * 下载: `git clone https://github.com/jedisct1/libsodium.git`
    * 当前使用版本：1.0.6
9. [必须] qt
    - 下载

            cd /home
            git clone git://git.gitorious.org/qt/qt5.git #（或者：git clone https://git.gitorious.org/qt/qt5.git）
            cd qt5
            git checkout 5.7
            perl init-repository

    - 更新已有qt：

            cd /home/qt5
            git pull
            perl init-repository -f --branch

    - 清理代码:

            git submodule foreach --recursive "git clean -dfx" && git clean -dfx

    详见:[开发文档](./Books/开发笔记.md#user-content-Qt编译)
10. [可选] 二维码编解码器
    1. libqrencode
        * 官网：http://fukuchi.org/works/qrencode/index.html.en
        * 下载：`git clone https://github.com/fukuchi/libqrencode.git`
         * 当前使用版本：v4.0.2
    2. qzxing
        * 主页：http://sourceforge.net/projects/qzxing
        * 下载:` git clone git://git.code.sf.net/p/qzxing/code`
        * 当前使用版本：
            git库版本 —— d61f2ac9821a1f52655092c588c498ca8db1f2c9
11. [可选] toxcore
    * 下载： `git clone git://github.com/irungentoo/toxcore.git`
    * 当前使用版本：e1089c1779fb1c58f17937108a6ba8c3d39573ae


### 本项目

* 主页：https://github.com/KangLin/rabbitim
* 下载：`git clone --recursive https://github.com/KangLin/rabbitim.git `

### 编译

1. [第三方依赖库](https://github.com/KangLin/RabbitThirdLibrary)目录介绍：

  
### 本工程编译
1. 用脚本  $(RabbitImRoot)/ThirdLibrary/build_rabbitim.sh 进行编译：

        . build_rabbitim.sh *平台参数* *[源码根目录]* *[工程工具]*

    + 平台参数：必须选择下面之一
        - android
        - windows_msvc
        - windows_mingw
        - unix
    + 源码根目录
    + 工程工具：可以选择 qmake 或 cmake 。 默认为 qmake
例如用qmake编译android平台：

        . build_rabbitim.sh android $(RabbitImRoot) qmake
2. 用Qt Creator 编译 
    1. 用 Qt Creator 编译 Rabbitim.pro
        * 用 Qt Creator 打开本工程根目录下的 Rabbitim.pro 文件。
        * 下载或自己编译第三方库，并放到 $(RabbitImRoot)/ThirdLibrary 目录下。
        * 打开菜单：文件->打开文件，选中 Rabbitim.pro 文件
        * 设置可选参数
            + QXMPP_USE_VPX=1             #使用 vpx
            + QXMPP_USE_SPEEX=1           #使用 speex
            + RABBITIM_USE_OPENCV=1       #使用 opencv
            + RABBITIM_USE_FFMPEG=1       #使用 ffmpeg
            + RABBITIM_USE_LIBCURL=1      #使用 libcurl
            + RABBITIM_USE_OPENSSL=1      #使用openssl
            + CONFIG+=static              #使用静态编译
            + RABBITIM_USE_DOXYGEN=1      #使用doxygen产生文档 
        * 项目->构建和运行->构建->构建环境，设置环境变更  
            GIT=${MSYS32_ROOT}/bin/git.exe                    #环境变量PATH中已设置GIT,则不需要
            PKG_CONFIG=${MSYS32_ROOT}/mingw32/bin/pkg-config  #**注意**，这里要用mingw32的pkg-config, 不能用msys2的pkg-config.国为不能识别路径符号“/”
            PKG_CONFIG_PATH=${RabbitImRoot}/ThirdLibrary/${PLATFORM}  #可选
            如果是交叉编译，还需要设置：
            export PKG_CONFIG_LIBDIR=${PKG_CONFIG_PATH} #可选
            export PKG_CONFIG_SYSROOT_DIR=${PKG_CONFIG_PATH} #可选
        * 构建->构建项目"RabbitIm"。编译本项目。
        * 运行：如果是在mingw32， 项目->构建和运行->运行->运行环境中设置
            PATH=${MSYS32_ROOT}/mingw32/bin;${RabbitImRoot}/ThirdLibrary/${PLATFORM}/bin
        * 构建->构建环境变量:设置 ANDROID_NDK_PLATFORM 到指定的API。例如：android-18 

    2. 用 Qt Createor 和 CMake 编译 CMakeLists.txt
        1. windows、linux平台
            * 用 Qt Creator 打开本工程根目录下的 CMakeLists.txt 文件。
            * 打开菜单：文件->打开文件，选中 CMakeLists.txt 文件
            * 会弹出一个执行 CMake 对话框
                - 如果是调试,在参数中填入:-DCMAKE_BUILD_TYPE=Debug 
                - 如果是发行,在参数中填入:-DCMAKE_BUILD_TYPE=Release 
                - 其它可选参数：
                  + -DOPTION_RABBITIM_USE_LIBCURL=ON
                  + -DOPTION_RABBITIM_USE_OPENSSL=ON
                  + -DOPTION_RABBITIM_USE_OPENCV=ON
                  + -DOPTION_RABBITIM_USE_STATIC=ON
                  + -OPTION_RABBITIM_DOXYGEN=ON  #使用doxygen生成文档，doxygen必须在环境变量PATH中
            * 选择相应的创建器,这里不能选错。
            * 点执行 CMake 按钮,开始执行 CMake 。如果成功，点完成就会打开项目。
            * 点调试，就可以编译，并调试程序
            * 注意，在 windows 下，不同的编译器与 Qt Creator 是绑定的。必须用它自带的 Qt Creator 才不会出错
        2. android平台
            * 用 Qt Creator 打开本工程根目录下的 CMakeLists.txt 文件。
            * 选择项目-> 相应平台的构建套件
            * 会弹出一个执行 CMake 对话框
                - 如果是调试,在参数中填入:-DCMAKE_BUILD_TYPE=Debug
                - 如果是发行,在参数中填入:-DCMAKE_BUILD_TYPE=Release
                - 其它可选参数：
                  + -DOPTION_RABBITIM_USE_LIBCURL=ON
                  + -DOPTION_RABBITIM_USE_OPENSSL=ON
                  + -DOPTION_RABBITIM_USE_OPENCV=ON
                  + -OPTION_RABBITIM_DOXYGEN=ON  #使用doxygen生成文档，doxygen必须在环境变量PATH中
                - 还要填入编译器参数：
                  + -DCMAKE_TOOLCHAIN_FILE=${RabbitImRoot}/platforms/android/android.toolchain.cmake
                  + -DQt5_DIR=${Qt5_DIR}
                ${Qt5_DIR}:qt for android 的 cmake 安装路径。例如：/c/Qt/Qt5.3.1/5.3/mingw482_32/lib/cmake/Qt5
            * 选择相应的创建器
            * 点执行 CMake 按钮,开始执行 CMake 。如果成功就会打开项目。

3. 用命令行编译
    1. 用 qmake 编译
        设置 qmake 路径到环境变量 PATH 中：
    
            QMAKE_PATH=${QT_INSTALL_DIR}/bin
            export PATH=$PATH:$QMAKE_PATH
    
        编译：
    
            mkdir build
            cd build
            qmake ${RabbitImRoot}/Rabbitim.pro  \
                    GIT=${MSYS32_ROOT}/bin/git.exe \
                    PKG_CONFIG=${MSYS32_ROOT}/mingw32/bin/pkg-config
            $(MAKE)
    
        MAKE在不同的环境下有不同的命令：  
          * make：unix、linux gun环境下用  
          * nmake：msvc环境下用  
          * mingw32-make：mingw 环境下用  
          * msys-make：msys 环境下用  
    2. 用 CMake 编译
        1. windows、linux平台
            * 用 G++ 编译
            
            进入项目源码根目录
            
                cd $(RabbitImRoot)
            
            建立编译目录
            
                mkdir rabbitim-build
            
            进入编译目录
            
                cd rabbitim-build
            
            设置qt环境变量[可选]
            - windows环境下可用这个批处理设置qt的环境变量
            
                ${QT_INSTALL_DIR}/bin/qtenv2.bat
            
            - linux下直接设置环境变量
            
                export PATH=$PATH:${Qt5_DIR} #windows环境mingw下设置 qt 到环境变量 PATH
            
            执行 camke
            
                cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DQt5_DIR=${Qt5_DIR}  
                cmake --build . --config Release   #执行编译
            
            启动程序
            
                ./RabbitIm
            
            参数说明：  
            Qt5_DIR:qt cmake 的安装路径。在qt安装目录的 ${QT_INSTALL_DIR}/lib/cmake/Qt5。
                     例如： /c/Qt/Qt5.3.1/5.3/mingw482_32/lib/cmake/Qt5
            
                cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DQt5_DIR=/c/Qt/Qt5.3.1/5.3/mingw482_32/lib/cmake/Qt5
                cmake --build . --config Release #编译
            
            * 用 msvc 编译
            
            启动 msvc 编译环境
            - 可以在开始菜单栏->vs2013->visual studio tools->VS2013 x86 本机工具命令提示
            - 也可以直接执行下面命令：
            
                "C:\Program Files\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
            
            设置qt的环境变量
            - windows 环境下可用这个批处理设置qt的环境变量
            
                C:\Qt\Qt5.3.1\5.3\msvc2013\bin\qtenv2.bat
            
            - 也可以直接设置环境变量
            
                set PATH=%PATH%;C:\Qt\Qt5.3.1\5.3\msvc2013\bin
            
            - 设置产生者：注意用 NMake Makefiles，如果用 VS，则需要对 CMakeLists.txt 中的目标路径做些修改
            
                cmake .. -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DQt5_DIR=C:\Qt\Qt5.3.1\5.3\msvc2013\lib\cmake\Qt5
            
            编译
            
                cmake --build . --config Release
4. 打包
    1. windows 下
      * 下载 nsis 并把它设置到环境变量（PATH）中（如果已经安装，请忽略此步)
    
          http://nsis.sourceforge.net/Download
          
      * 再执行下面命令
      
          set PATH=%PATH%;${QT_INSTALL_DIR}/bin #把qt安装目录加到环境变量中  
          cpack -G NSIS   #这个是编译完成后，单独打包的命令，或者也可用下面编译时打包命令  
          cmake --build .  --config Release --target package  #编译时同时打包  
          
    2. unix、linux 下
      * rpm 需要安装 rpmbuilder
      * 再执行下面命令
    
        export PATH=$PATH:${QT_INSTALL_DIR}/bin  
        cpack -G deb rmp
5. android 平台:

windows 平台下以 mingw 环境为例

###### 2.2.3.1. 先设置环境变量：

    export ANDROID_NDK_ROOT=     #android ndk 根目录
    export ANDROID_NDK=$ANDROID_NDK_ROOT
    export ANDROID_SDK=                   #android sdk 根目录
    export ANDROID_SDK_ROOT=$ANDROID_SDK
    export JAVA_HOME=                       #jdk根目录
    export ANT_ROOT=                         #ant工具的目录
    export PATH=$PATH:%ANT_ROOT/bin

###### 2.2.3.2. 建立编译目录：

    cd $(RabbitImRoot)    #进入项目源码根目录
    mkdir rabbitim-build  #建立编译目录
    cd rabbitim-build         #进入编译目录

###### 2.2.3.3. 配置、编译：

    cmake .. -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../platforms/android/android.toolchain.cmake \
        -DCMAKE_MAKE_PROGRAM=${ANDROID_MAKE} \  #windows 下需要 make 工具的位置,linux下则不需要这个
        -DQt5_DIR=${Qt5_DIR}         \   #qt 的 cmake 位置
        -DANT=${ANT_ROOT}/bin/ant    \   #ant 工具位置
    cmake --build .       #执行编译

变量说明：  

    ${CMAKE_TOOLCHAIN_FILE}:编译工具链文件。在源码根目录 platforms/android/android.toolchain.cmake
    ${ANDROID_MAKE}:windows下android ndk 自带的make工具，如果在windows平台上编译，则需要指定。例如："$ANDROID_NDK/prebuilt/windows/bin/make"
    ${Qt5_DIR}:qt for android 的 cmake 安装路径。位于qt for android的安装目录 lib/cmake/Qt5 。例如：Qt5_DIR=/c/Qt/Qt5.3.1_android/5.3/android_armv7/lib/cmake/Qt5
    ${ANT_ROOT}:ant 的安装路径（linux一般会安装在 /use/bin 下）。例如： ANT_ROOT=/d/software/apache-ant-1.9.4

例如：

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../rabbitim/platforms/android/android.toolchain.cmake -DQt5_DIR=/c/Qt/Qt5.3.1_android/5.3/android_armv7/lib/cmake/Qt5 ../rabbitim -DANT=/d/software/apache-ant-1.9.4/bin/ant -DCMAKE_MAKE_PROGRAM=/d/software/android-ndk-r9/prebuilt/windows/bin/make -G"Unix Makefiles"

###### 2.2.3.4. 可以会出现下面错误：

CMake Error at c:/Qt/Qt5.3.1/5.3/android_armv7/lib/cmake/Qt5Gui/Qt5GuiConfig.cma
ke:15 (message):
  The imported target "Qt5::Gui" references the file

     "Qt5Gui_EGL_LIBRARY-NOTFOUND"

  but this file does not exist.  Possible reasons include:

  * The file was deleted, renamed, or moved to another location.

  * An install or uninstall procedure did not complete successfully.

  * The installation package was faulty and contained

     "c:/Qt/Qt5.3.1/5.3/android_armv7/lib/cmake/Qt5Gui/Qt5GuiConfigExtras.cmake"

  but not all the files it references.

Call Stack (most recent call first):
  c:/Qt/Qt5.3.1/5.3/android_armv7/lib/cmake/Qt5Gui/Qt5GuiConfigExtras.cmake:31 (_qt5_Gui_check_file_exists)  
  c:/Qt/Qt5.3.1/5.3/android_armv7/lib/cmake/Qt5Gui/Qt5GuiConfigExtras.cmake:58 (_qt5gui_find_extra_libs)  
  c:/Qt/Qt5.3.1/5.3/android_armv7/lib/cmake/Qt5Gui/Qt5GuiConfig.cmake:143 (include)  
  c:/Qt/Qt5.3.1/5.3/android_armv7/lib/cmake/Qt5/Qt5Config.cmake:26 (find_package)  


* 原因：
没找到 Qt5Gui_EGL 库。预编译的android包中没包含此库。
* 解决办法：
把 c:/Qt/Qt5.3.1/5.3/android_armv7/lib/cmake/Qt5Gui/Qt5GuiConfigExtras.cmake 中第 58 行注释掉。

翻译文件部署
------------
1. 工具->外部->Qt 语言家->发布翻译(lrelease)，生成 *.pm 文件。
2. 把 pm 文件复制到安装目录的 translate 目录下。

调试
-----

xmpp服务配置域名（rabbitim.com），客户端如果没有域名，连接服务器时，则直接设置主机IP，同时设置域名。  
当然，也可以修改系统hosts文件配置域名解析。这个域名实质上是xmpp应用程序用于区别不同域。
