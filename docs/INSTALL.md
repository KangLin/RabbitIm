工具：
-----

##### 1. bash 环境:

* windows下：msys（msys2） 或者 cygwin
    * msys: http://www.mingw.org/wiki/MSYS
    * msys2: http://sourceforge.net/projects/msys2/
        代码位置: https://github.com/Alexpux/MSYS2-packages
    * cygwin主页: http://www.cygwin.org/

##### 2. 下载工具：

* wget：

##### 3. 脚本工具：

* python:2.7.6  版本 2.7 或以后
    + 主页: https://www.python.org/  http://www.activestate.com/activepython/
* perl：5.18.2  版本 5.12 或以后
    + 主页： http://www.perl.org/  http://www.activestate.com/activeperl/
* bash:msys或cygwin
* ruby:qtwebkit编译需要 版本 1.9.3 或以后
    * 主页：
        * http://www.ruby-lang.org/
        * http://rubyinstaller.org/  
        * https://github.com/ruby/ruby

##### 4. cmake：

* 主页：http://www.cmake.org/
* 当前使用版本：3.1 

##### 5. 安装auto工具：

1. automake: http://www.gnu.org/software/automake/
2. autoconf: http://www.gnu.org/software/autoconf/
3. libtool: http://www.gnu.org/software/libtool/
4. m4: http://www.gnu.org/software/m4
5. pkg-config: http://www.freedesktop.org/wiki/Software/pkg-config/
6. make:根据bash系统的不同，有msys make（msys bash）、mingw make、gnu make（cygwin bash）

##### 6. 版本控制工具：

* subversion: http://subversion.apache.org/
* git: http://www.git-scm.com/

##### 7. windows平台工具：

1. msvc：
    * 主页：http://msdn.microsoft.com/zh-cn/vstudio
    * 当前使用版本：vs 2013 update 4
2. windows sdk(The Microsoft® Windows® Software Development Kit (SDK) for Windows 8.1):
    https://msdn.microsoft.com/zh-cn/windows/desktop/bg162891
3. Windows Driver Kit:
    http://www.microsoft.com/en-us/download/confirmation.aspx?id=42273
4. bash 环境
    * msys: http://www.mingw.org/wiki/MSYS
    * msys2: http://sourceforge.net/projects/msys2/
        代码位置: https://github.com/Alexpux/MSYS2-packages
    * cygwin主页: http://www.cygwin.org/

##### 8. 汇编工具（yasm、nasm）：

* [yasm](http://yasm.tortall.net/)
* [nasm](http://www.nasm.us/)  
    下载并安装，并且设置路径到环境变量PATH

##### 9. 编译工具：

1. windows
    * msvc
    * mingw gcc
2. linux
    * gcc、g++
    当前使用版本：4.8
3. android
    * android ndk

##### 10. android工具：

1. Java jdk：http://www.oracle.com/technetwork/java/javase/downloads/index.html
2. ant：http://ant.apache.org/
3. android sdk：http://developer.android.com/sdk/index.html
4. android ndk：http://developer.android.com/ndk/index.html

##### 11. 安装工具

1. deb打包工具：dh_make、 debhelper
2. rpm打包工具:
3. windows安装工具:nsis

##### 12. icu工具，编译 QtWebkit 需要

##### 13. [Qt工具与版本](http://qt-project.org/wiki/Qt-5.5.0-tools-and-versions)

##### 14. QT：

* 主页：http://qt-project.org/
* 下载：http://qt-project.org/downloads
* 当前使用版本：5.3.0
    + QT开发工具参考：http://qt-project.org/doc/qt-4.8/developing-with-qt.html
    + [QT编译](http://blog.csdn.net/kl222/article/details/44216951)

第三方库：
--------

#### 1. XMPP协议（QXMPP）：

* 主页：http://code.google.com/p/qxmpp  
* 需要用我修改的版本：https://github.com/KangLin/qxmpp
* 下载：

    git clone https://github.com/KangLin/qxmpp.git

* 当前使用版本：  
    git库版本 —— 0b49b87c2c4e38a6d2fd9983949e34782575955c  
    编译方法参考源码根目录下的README文件。

#### 2. webrtc库：

* 主页：http://www.webrtc.org/  
* 下载：

    gclient config http://webrtc.googlecode.com/svn/trunk  
    gclient sync --force  

* 当前使用版本：svn库版本——r5663  
* 编译参考：http://blog.csdn.net/kl222/article/details/17198873

#### 3. 编解码库（ffmpeg）：

* 主页：http://www.ffmpeg.org/  
* 下载：

    git clone git://source.ffmpeg.org/ffmpeg.git

*当前使用版本：
    git库版本 —— n2.6.2

#### 4. 编解码库(x264)

* 下载：`git clone git://git.videolan.org/x264.git`  
* 当前使用版本：  
    git库版本 —— 121396c

#### 5. 解码库(libvpx)

* 下载：`git clone https://chromium.googlesource.com/webm/libvpx libvpx`  
* 当前使用版本:  
    git库版本 —— 9b99eb2

#### 6. 音频处理库(libspeexdsp)

* 下载：`git clone http://git.xiph.org/speexdsp.git`  
* 当前使用版本:  
    git库版本 —— 887ac10

#### 7. 音频编解码库（libspeex）

* 下载：`git clone http://git.xiph.org/speex.git`  
* 当前使用版本：  
    git库版本 —— 6aab25c

#### 8. [图像处理库(opencv)](http://opencv.org/)

* 下载：`git clone git://github.com/Itseez/opencv.git`  
* 当前使用版本:  
    git库版本 —— 2.4.11

#### 9. libyuv

* 下载:`git clone http://git.chromium.org/external/libyuv.git`
* 当前使用版本:  
    git库版本 —— 416c48d

#### 10. [openssl](http://www.openssl.org/)

* 下载：`git clone  https://github.com/openssl/openssl`  
* 当前使用版本：  
    git库版本 —— c8ece53  
	remotes/origin/OpenSSL_1_0_2-stable

#### 11. [libcurl](http://curl.haxx.se)

* 下载：`git://github.com/bagder/curl.git`  
* 当前使用版本：  
    git库版本 —— curl-7_41_0

#### 12. [libicu](http://site.icu-project.org/)

* 下载：`svn co http://source.icu-project.org/repos/icu/icu/trunk/ icu`
* 当前使用版本：
    Revision: 37321

#### 13. qt

    cd /home
    git clone git://git.gitorious.org/qt/qt5.git #（或者：git clone https://git.gitorious.org/qt/qt5.git）
    cd qt5
    git checkout 5.5
    perl init-repository

更新已有qt：

    cd /home/qt5
    git pull
    perl init-repository -f --branch

本项目：
---------

* 主页：https://github.com/KangLin/rabbitim
* 下载：`git clone https://github.com/KangLin/rabbitim.git `

编译：
-----

### 第三方依赖库目录：

在源码根目录下建立第三方库目录：ThirdLibary  
把第三方依赖库编译成功后，放到ThirdLibary目录下,ThirdLibary目录结构如下：

    ThirdLibary
        ｜
        ｜-----patch            #补丁包目录
        ｜-----build_script     #第三方库编译脚本目录
        ｜-----src              #第三方库源码目录
        ｜-----windows_msvc     # Windows 平台 msvc 编译的第三方库
        ｜       ｜-----include #头文件
        ｜       ｜-----lib     #库文
        ｜-----windows_mingw    # Windows 平台 mingw 交叉编译的第三方库
        ｜       ｜-----include
        ｜       ｜-----lib
        ｜------unix            # unix 平台的第三方库
        ｜       ｜-----include
        ｜       ｜-----lib
        ｜-----android          # android 平台的第三方库
        ｜       ｜-----include
        ｜       ｜-----lib
        ｜------windows_phone   #Windows Phone 平台的第三方库
        ｜       ｜------include
        ｜       ｜------lib
        ｜------ ios            # ios 平台的第三方库
        ｜       ｜------include
        ｜       ｜------lib

可以在 http://pan.baidu.com/s/1ntA0t5n 中下载本项目依赖的预编译好的第三方库。

### 第三方依赖库编译脚本
第三库编译脚本是 bash 脚本。运行这些脚本时，需要有 bash 环境。linux、unix 默认安装了 bash 环境。
windows 下需要安装 cygwin 或者 msys 环境。  

环境变量：

    export RabbitImRoot=/home/rabbitim    #本工程源码根目录

所需要的环境变量可以保存到系统配置文件 ~/.profile 文件中。作为全局环境变量。但这可能会与其它工程需要的环境变量冲突。
为了避免这个问题。你也可以把环境变量保到 build_envsetup_${RABBITIM_BUILD_TARGERT}.sh 文件中。

脚本只编译第三方依赖库的发行版本。如果你需要调试版本，请手工编译。

第三方库编译脚本位于：${RabbitImRoot}/ThirdLibary/build_script 目录下。  

##### 1. android 平台：
    * build_envsetup_android.sh : 设置编译时需要的变量  
    * build.sh android [source_code_directory] : 编译第三方库脚本
        环境变量说明:
    + export ANDROID_NDK_ROOT=              #android ndk 根目录
    + export ANDROID_NDK=$ANDROID_NDK_ROOT
    + export ANDROID_SDK=                   #android sdk 根目录
    + export ANDROID_SDK_ROOT=$ANDROID_SDK    
    + export JAVA_HOME=                     #jdk根目录 
    + export ANT_ROOT=                      #ant工具的目录
    + QT_ROOT=                              #qt的安装位置
    + RABBITIM_MAKE_JOB_PARA="-j2"  #make 同时工作进程参数,建议设置为你机器CUP个数
    + JOM=make #设置 QT make 工具 JOM
    所需要的环境变量可以保存到系统配置文件 ~/.profile 文件中。作为全局环境变量。但这可能会与其它工程需要的环境变量冲突。
    为了避免这个问题。你也可以把环境变量保到 build_envsetup_${RABBITIM_BUILD_TARGERT}.sh 文件中。  
    再运行:

    ${RabbitImRoot}/ThirdLibary/build_script/build.sh android [source_code_directory] #进行第三方依赖库编译

##### 2. windows 平台：
   windows 平台有两种方法编译：
   1. ) msvc 工具链：
      * build_envsetup_windows_msvc.sh:设置编译时需要的变量  
      * build.sh windows_msvc [source_code_directory] : 编译第三方库脚本
        环境变量说明： 
        + QT_ROOT=/home/k/Qt5.4.1/5.4/gcc_64 #QT 安装根目录
        + RABBITIM_MAKE_JOB_PARA="-j2"  #make 同时工作进程参数,建议设置为你机器CUP个数
        + JOM=nmake #设置 QT make 工具 JOM
   msvc 工具链的环境变量可用下面方法设置：  
   先从菜单栏中起动vs2013编译命令行工具：  
   C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts  
   VS2013 x86 本机工具命令提示  
   在命令行下，启动msys。 
   `c:\MinGW\msys\1.0\msys.bat` 
   如果用msys2：`c:\msys32\mingw32_shell.bat`  
   注意，msys中不要装link工具，否则会导致出错。如果有link工具，暂时把它命名成其它名称。  
   然后再进入脚本目录：`cd ${RabbitImRoot}/ThirdLibary/build_script`。再运行你想运行的编译脚本。例如： `./build.sh` 。
   2. )mingw 工具链
      * build.sh windows_mingw [source_code_directory] : 编译第三方库脚本
      * build_envsetup_windows_mingw.sh:设置编译时需要的变量
        环境变量说明： 
            + QT_ROOT=/home/k/Qt5.4.1/5.4/gcc_64 #QT 安装根目录
            + RABBITIM_MAKE_JOB_PARA="-j2"  #make 同时工作进程参数,建议设置为你机器CUP个数
            + JOM=make #设置 QT make 工具 JOM

##### 3. windows phone 平台：

### 第三方依赖库编译
##### 1. 编解码库(libvpx)编译：  
详见：http://blog.csdn.net/kl222/article/details/23101115  
安装装到：

        make PREFIX=${RabbitImRoot}/ThirdLiabary/${Platform}

##### 2. ffmpeg编译：  
详见《ffmpeg教程》

##### 3. opencv编译：  
[Building OpenCV4Android from trunk](http://code.opencv.org/projects/opencv/wiki/Building_OpenCV4Android_from_trunk)  
详见:[http://blog.csdn.net/kl222/article/details/27223791](http://blog.csdn.net/kl222/article/details/27223791)  
编译完成后,安装到:${RabbitImRoot}/ThirdLiabary/${Platform}  

##### 4. libspeex、libspeexdsp编译:

       ./autogen.sh  
       configure --prefix=${RabbitImRoot}/ThirdLiabary/${Platform}  
       make install  

##### 5. webrtc编译：
详见[《webrtc教程》](http://blog.csdn.net/kl222/article/details/17198873)

##### 6. libyuv 编译：
官网：https://code.google.com/p/libyuv/wiki/GettingStarted  
详见：[《libyuv编译》](http://blog.csdn.net/kl222/article/details/41309541)  
或者： [libyuv编译.txt](./Books/libyuv编译.txt)

##### 7. [openssl编译](http://blog.csdn.net/kl222/article/details/41787529)

##### 8. [libcurl 编译](http://blog.csdn.net/kl222/article/details/41898025)

##### 9. QXMPP编译：  
1. 用 Qt Creator 进行编译（详见源码根目录下的README文件）：
    * 用 Qt Creator 打开本工程。
    * 打开“文件->打开文件或项目”。
    * 在弹出的对话框中选中qxmpp.pro，打开qxmpp工程。
    * 点左边工具栏中的“项目”，选择qxmpp标签，在相应平台“构建套件”中修改“构建步骤”参数，
           在“构建步骤”中的“额外参数”中，加入 “PREFIX=$(RabbitImRoot)/ThirdLibary/${RABBITIM_BUILD_TARGERT}”，
           其中$(RabbitImRoot)是本项目源码的根目录，在下面的“构建环境”变量中添加这个环境变量。
           当然，也可以直接在“额外参数”中把$(RabbitImRoot)替换成本项目源码根目录路径。
           ${RABBITIM_BUILD_TARGERT}为相应的平台，可以为windows、android、unix、ios。
           如果需要编译成静态库，需要在额外参数中加入：
           QXMPP_LIBRARY_TYPE=staticlib 。在本项目编译时连接静态 qxmpp 库需要增加 -DQXMPP_STATIC 。
    * 设置编解码器：现在QXMPP支持vpx、THEORA视频编解码器；G711u、SPEEX音频编解码器。音频默认为G711u。
           视频无默认编解码器，所以如果需要视频，必须指定第三方视频编解码器。
           以libvpx为例：在额外参数中填入QXMPP_USE_VPX=1
           并且添加libvpx库位置:INCLUDEPATH+=$(RabbitImRoot)/ThirdLibary/${RABBITIM_BUILD_TARGERT}/include
           LIBS+=-L$(RabbitImRoot)/ThirdLibary/${RABBITIM_BUILD_TARGERT}/lib
    * 选择windows或linux平台，在相应平台“构建套件”中的“运行”标签，
           部署->详情->部署->添加部署步骤->选择make命令->Make参数中加上"install"。
           其它平台可能会有平台自己的部署步骤，所以不能在部署这里安装。可以用下面方法：
           在相应平台“构建套件”中的“构建”标签，“构建步骤”->“make”->“make参数”中加上install。
           如果修改了代码，这种方法需要重编译。
    * 在“项目”->本项目中的“依赖关系”标签中选中qxmpp。
    * 在项目浏览器中选中qxmpp项目，右键点击“执行qmake”；再右键点击“构建”；再右键点击“部署”。
           在部署时会出现":-1: error: [install_htmldocs] Error 4 (ignored)"错误。
           这是由于没有安装doxygen，所以不能产生帮助文档。可以忽略。
    * 当前版本有BUG，需要打下面补丁，或者直接下我改过的版本:`git clone git@github.com:KangLin/qxmpp.git`
        $(RabbitImRoot)/ThirdLibary/patch/0001-add-handle-non-sasl-authentication-error-response.patch  
          + 建立分支 patch:`git branch patch`
          + 切换到分支 patch:`git checkout patch`
          + 应用补丁:`git am $(RabbitImRoot)/ThirdLibary/patch/0001-add-handle-non-sasl-authentication-error-response.patch `
          + 切换到主分支：`git checkout master`
          + 把 patch 分支合并到master 分支中 :`git merge patch`
2. 用命令行编译：  
     * 设置 qmake 路径到环境变量 PATH 中：`export PATH=$PATH:$QMAKE_PATH`  
     * 建立编译目录：`mkdir build; cd build`
     * 生成工程文：`qmake -o Makefile QXMPP_LIBRARY_TYPE=staticlib \
                 PREFIX=$(RabbitImRoot)/ThirdLibary/${RABBITIM_BUILD_TARGERT} \
                 INCLUDEPATH+=$(RabbitImRoot)/ThirdLibary/${RABBITIM_BUILD_TARGERT}/include \
                 LIBS+=-L$(RabbitImRoot)/ThirdLibary/${RABBITIM_BUILD_TARGERT}/lib \
                 QXMPP_USE_VPX=1 \
                 ${RabbitImRoot}/Rabbitim.pro`
     * 编译：`$(MAKE)`  
        MAKE在不同的环境下有不同的命令：
        + make：unix、linux gun环境下用
        + nmake：msvc环境下用
        + mingw32-make：mingw 环境下用
        + msys-make：msys 环境下用

##### 10. [qt编译](http://blog.csdn.net/kl222/article/details/44216951)

本工程编译
--------

如果要连接静态 QXMPP 库时，需要加上-DQXMPP_STATIC 。  
用脚本  $(RabbitImRoot)/ThirdLibary/build_rabbitim.sh 进行编译：
. build_rabbitim.sh (android|windows_msvc|windows_mingw|unix) SOURCE_CODE_ROOT [qmake]

#### 1. 用Qt Creator 编译 

##### 1.1. 用 Qt Creator 编译 Rabbitim.pro
用 Qt Creator 打开本工程根目录下的 Rabbitim.pro 文件。
  * 下载(http://pan.baidu.com/s/1ntA0t5n )或自己编译第三方库，并放到 $(RabbitImRoot)/ThirdLibary 目录下。
  * 打开菜单：文件->打开文件，选中 Rabbitim.pro 文件
  * 设置可选参数
         + QXMPP_USE_VPX=1              #使用 vpx
         + QXMPP_USE_SPEEX=1            #使用 speex
         + RABBITIM_USER_OPENCV=1       #使用 opencv
         + RABBITIM_USER_FFMPEG=1       #使用 ffmpeg
         + RABBITIM_USER_LIBCURL=1      #使用 libcurl
         + RABBITIM_USER_OPENSSL=1      #使用openssl
  * 构建->构建项目"RabbitIm"。编译本项目。

##### 1.2. 用 Qt Createor 和 CMake 编译 CMakeLists.txt

###### 1.2.1. windows、linux平台
用 Qt Creator 打开本工程根目录下的 CMakeLists.txt 文件。
  * 打开菜单：文件->打开文件，选中 CMakeLists.txt 文件
  * 会弹出一个执行 CMake 对话框
    + 如果是调试,在参数中填入:-DCMAKE_BUILD_TYPE=Debug 
    + 如果是发行,在参数中填入:-DCMAKE_BUILD_TYPE=Release 
    + 其它可选参数：
    -DOPTION_RABBITIM_USER_LIBCURL=ON
    -DOPTION_RABBITIM_USER_OPENSSL=ON
    -DOPTION_RABBITIM_USER_OPENCV=ON
  * 选择相应的创建器,这里不能选错。
  * 点执行 CMake 按钮,开始执行 CMake 。如果成功，点完成就会打开项目。
  * 点调试，就可以编译，并调试程序
  * 注意，在 windows 下，不同的编译器与 Qt Creator 是绑定的。必须用它自带的 Qt Creator 才不会出错

###### 1.2.2. android平台
用 Qt Creator 打开本工程根目录下的 CMakeLists.txt 文件。
  * 选择项目-> 相应平台的构建套件
  * 会弹出一个执行 CMake 对话框
    + 如果是调试,在参数中填入:-DCMAKE_BUILD_TYPE=Debug
    + 如果是发行,在参数中填入:-DCMAKE_BUILD_TYPE=Release
    + 其它可选参数：
        -DOPTION_RABBITIM_USER_LIBCURL=ON
        -DOPTION_RABBITIM_USER_OPENSSL=ON
        -DOPTION_RABBITIM_USER_OPENCV=ON
    + 还要填入编译器参数：-DCMAKE_TOOLCHAIN_FILE=${RabbitImRoot}/platforms/android/android.toolchain.cmake -DQt5_DIR=${Qt5_DIR}
    ${Qt5_DIR}:qt for android 的 cmake 安装路径。例如：/c/Qt/Qt5.3.1/5.3/mingw482_32/lib/cmake/Qt5
  * 选择相应的创建器
  * 点执行 CMake 按钮,开始执行 CMake 。如果成功就会打开项目。

#### 2. 用命令行编译

##### 2.1. 用 qmake 编译
**设置 qmake 路径到环境变量 PATH 中**：`export PATH=$PATH:$QMAKE_PATH`，QMAKE_PATH=${QT_INSTALL_DIR}/bin

    mkdir build
    cd build
    qmake ${RabbitImRoot}/Rabbitim.pro
    $(MAKE)

MAKE在不同的环境下有不同的命令：  
    * make：unix、linux gun环境下用  
    * nmake：msvc环境下用  
    * mingw32-make：mingw 环境下用  
    * msys-make：msys 环境下用  

##### 2.2. 用 CMake 编译
###### 2.2.1. windows、linux平台
    * 用 G++ 编译

    cd $(RabbitImRoot)    #进入项目源码根目录
    mkdir rabbitim-build  #建立编译目录
    cd rabbitim-build        #进入编译目录
    ${QT_INSTALL_DIR}/bin/qtenv2.bat   #[可选]windows环境下可用这个批处理设置qt的环境变量，linux下直接设置环境变量
    cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DQt5_DIR=${Qt5_DIR}    #执行 camke
    cmake --build .   --config Release      #执行编译
    ./RabbitIm               #启动程序

    Qt5_DIR:qt cmake 的安装路径。在qt安装目录的 ${QT_INSTALL_DIR}/lib/cmake/Qt5。
    例如： /c/Qt/Qt5.3.1/5.3/mingw482_32/lib/cmake/Qt5

    export PATH=$PATH:/c/Qt/Qt5.3.1/5.3/mingw482_32/bin #windows环境mingw下设置 qt 到环境变量 PATH
    cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DQt5_DIR=/c/Qt/Qt5.3.1/5.3/mingw482_32/lib/cmake/Qt5
    cmake --build . --config Release #编译

    * 用 msvc 编译

    #启动 msvc 编译环境,可以在开始菜单栏->vs2013->visual studio tools->VS2013 x86 本机工具命令提示，也可以直接执行下面命令：
    "C:\Program Files\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
    #windows 环境下可用这个批处理设置qt的环境变量
    C:\Qt\Qt5.3.1\5.3\msvc2013\bin\qtenv2.bat
    #也可以直接设置环境变量
    set PATH=%PATH%;C:\Qt\Qt5.3.1\5.3\msvc2013\bin
    #注意产生者用 NMake Makefiles，如果用 VS，则需要对 CMakeLists.txt 中的目标路径做些修改
    cmake .. -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DQt5_DIR=C:\Qt\Qt5.3.1\5.3\msvc2013\lib\cmake\Qt5
    #编译
    cmake --build . --config Release

###### 2.2.2. 打包
    + windows 下
        * 下载 nsis 并把它设置到环境变量（PATH）中（如果已经安装，请忽略此步)

    http://nsis.sourceforge.net/Download

        * 再执行下面命令

    set PATH=%PATH%;${QT_INSTALL_DIR}/bin #把qt安装目录加到环境变量中
    cpack -G NSIS   #这个是编译完成后，单独打包的命令，或者也可用下面编译时打包命令
    cmake --build .  --config Release --target package     #编译时同时打包

    + unix、linux 下
        * rpm 需要安装 rpmbuilder
    
        * 再执行下面命令

    export PATH=$PATH:${QT_INSTALL_DIR}/bin
    cpack -G deb rmp

###### 2.2.3. android 平台:

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

翻译文件部署：
------------
1. 工具->外部->Qt 语言家->发布翻译(lrelease)，生成 *.pm 文件。
2. 把 pm 文件复制到安装目录的 translate 目录下。

调试：
-----

xmpp服务配置域名（rabbitim.com），客户端如果没有域名，连接服务器时，则直接设置主机IP，同时设置域名。  
当然，也可以修改系统hosts文件配置域名解析。这个域名实质上是xmpp应用程序用于区别不同域。

各目标编译详细说明：
----------------

1. [ubuntu](INSTALL_UBUNTU.md)
2. [android](INSTALL_ANDROID.md)
3. [windows](INSTALL_WINDOWS.md)
