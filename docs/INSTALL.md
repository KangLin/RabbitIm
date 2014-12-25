依赖：
-----

* 开发工具：git、svn、autoconf、automake、make、cmake、python、perl、bash、
* Windows 下开发环境:Mingw、msys（或者cygwin）、msvc
* android 开发工具：android ndk、android sdk、jdk、ant
* 汇编工具：yasm
* UI：QT
* 即时通信协议（XMPP）：QXMPP
* 视频通信：webrtc
* 编解码库：ffmpeg、x264（可选）、libvpx、libyuv（可选）、libspeexdsp（可选）、
           libspeex（可选）
* 图形处理库：opencv（可选）

下载工具和依赖：
-------------

1. QT：
主页：http://qt-project.org/  
下载：http://qt-project.org/downloads  
当前使用版本：5.3.0  
QT开发工具参考：http://qt-project.org/doc/qt-4.8/developing-with-qt.html

2. msvc：
主页：http://msdn.microsoft.com/zh-cn/vstudio
当前使用版本：vs 2013 update 4

3. mingw、msys（或者cygwin）：
mingw主页：http://www.mingw.org/ 。这个主页好象不是最新的，请用：http://sourceforge.net/projects/mingw-w64/  
cygwin主页：http://www.cygwin.org/  
当前使用的是 mingw ，因为 QT for android in windows 预编译包中已包含了 mingw 。只需要下载 msys 。

4. cmake：
主页：http://www.cmake.org/
当前使用版本：3.0.2 

5. XMPP协议（QXMPP）：
主页：http://code.google.com/p/qxmpp  
下载：
`git clone  https://github.com/qxmpp-project/qxmpp.git`  
当前使用版本：  
    git库版本 —— v0.8.1    
编译方法参考源码根目录下的README文件。

6. webrtc库：
主页：http://www.webrtc.org/  
下载：  
        gclient config http://webrtc.googlecode.com/svn/trunk  
        gclient sync --force  
当前使用版本：svn库版本——r5663  
编译参考：http://blog.csdn.net/kl222/article/details/17198873

7. 编解码库（ffmpeg）：  
主页：http://www.ffmpeg.org/  
下载：
        git clone git://source.ffmpeg.org/ffmpeg.git
当前使用版本：  
    git库版本 —— 6e6b0a8eed409c06d30cbc86a8e940f8779a4ac2  
    
8. 编解码库(x264)
下载：`git clone git://git.videolan.org/x264.git`  
当前使用版本：  
    git库版本 —— 40bb56814e56ed342040bdbf30258aab39ee9e89

9. 解码库(libvpx)
下载：`git clone https://chromium.googlesource.com/webm/libvpx libvpx`  
当前使用版本:  
    git库版本 —— 8619274ea384500abeb698f832815ca892266954

10. 音频处理库(libspeexdsp)
下载：`git clone http://git.xiph.org/speexdsp.git`  
当前使用版本:  
    git库版本 —— a6930dde69ed37c0d56f8fe5b4394bb3e9d31e87

11. 音频编解码库（libspeex）
下载：`git clone http://git.xiph.org/speex.git`  
当前使用版本：  
    git库版本 —— 99446819ea60d39dcabac70cb22fac6ba23fdbbe

12. [图像处理库(opencv)](http://opencv.org/)
下载：`git clone git://github.com/Itseez/opencv.git`  
当前使用版本:  
    git库版本 —— dce629d0e25598d07723324de300ae1cad234f13

13. libyuv  
下载:`git clone http://git.chromium.org/external/libyuv.git`  
当前使用版本:  
    git库版本 —— 002feab4c5d07828ab1c7982351ce08e833a0990

14. [openssl](http://www.openssl.org/)
下载：`git clone  https://github.com/openssl/openssl`
当前使用版本：
    git库版本 —— OpenSSL_1_0_1j

15. [libcurl](http://curl.haxx.se)
下载：`git://github.com/bagder/curl.git`  
当前使用版本：  
    git库版本 —— curl-7_39_0

编译：
-----

### 第三方依赖库编译

在源码根目录下建立第三方库目录：ThirdLibary  
把第三方依赖库编译成功后，放到ThirdLibary目录下,ThirdLibary目录结构如下：

    ThirdLibary
        ｜
        ｜-----patch            #补丁包目录
        ｜-----build_script     #第三方库编译脚本目录
        ｜-----src              #源码目录
        ｜-----windows_msvc     # Windows 平台 msvc 编译器的第三方库
        ｜       ｜-----include #头文件
        ｜       ｜-----lib     #库文
        ｜-----windows_mingw    # Windows 平台 mingw 编译器的第三方库
        ｜       ｜-----include
        ｜       ｜-----lib
        ｜------windows_phone   #Windows Phone 平台的第三方库
        ｜       ｜------include
        ｜       ｜------lib
        ｜-----android          # android 平台的第三方库
        ｜       ｜-----include
        ｜       ｜-----lib
        ｜------unix            # unix 平台的第三方库
        ｜       ｜-----include
        ｜       ｜-----lib
        ｜------ ios            # ios 平台的第三方库
        ｜       ｜------include
        ｜       ｜------lib

可以在 http://pan.baidu.com/s/1ntA0t5n 中下载本项目依赖的预编译好的第三方库。

#### 第三方库编译脚本说明
第三库编译脚本是 bash 脚本。运行这些脚本时，需要有 bash 环境。linux、unix 默认安装了 bash 环境。
windows 下需要安装 cygwin 或者 msys 环境。  

环境变量：

    export RabbitImRoot=/home/rabbitim    #本工程源码根目录
    
所需要的环境变量可以保存到系统配置文件 ~/.profile 文件中。作为全局环境变量。但这可能会与其它工程需要的环境变量冲突。
为了避免这个问题。你也可以把环境变量保到 build_${PLATFORM}_envsetup.sh 文件中。

脚本只编译第三方依赖库的发行版本。如果你需要调试版本，请手工编译。

第三方库编译脚本位于：${RabbitImRoot}/ThirdLibary/build_script 目录下。  

1. android 平台：
    * build_android.sh : 编译所有第三方依赖库
    * build_android_${LIBRARY} : 编译具体的第三方库。它可以带一个指定第三方库源码根目录作为输入参数。如果不指定这个参数，
                                那么将会取 ${RabbitImRoot}/ThirdLibary/src/${LIBRARY} 作为第三方库源码根目录。
                                例如：我只要编译 libvpx 库，则可以直接运行：
                                `build_android_libvpx.sh [libvpx源码根目录]`
    * build_android_envsetup.sh : 设置编译时需要的变量  
        环境变量说明:
    + export ANDROID_NDK_ROOT=              #android ndk 根目录
    + export ANDROID_NDK=$ANDROID_NDK_ROOT
    + export ANDROID_SDK=                   #android sdk 根目录
    + export ANDROID_SDK_ROOT=$ANDROID_SDK    
    + export JAVA_HOME=                     #jdk根目录 
    + QMAKE=                                #设置用于 mingw 平台编译的 QMAKE
    所需要的环境变量可以保存到系统配置文件 ~/.profile 文件中。作为全局环境变量。但这可能会与其它工程需要的环境变量冲突。
    为了避免这个问题。你也可以把环境变量保到 build_${PLATFORM}_envsetup.sh 文件中。  

2. windows 平台：
   windows 平台有两种方法编译：
   1. ) msvc 工具链：
      * build_windows_msvc.sh:编译所有第三依赖库
      * build_windows_msvc_${LIBRARY}: 编译具体的第三方库。它可以带一个指定第三方库源码根目录作为输入参数。如果不指定这个参数，
                                    那么将会取 ${RabbitImRoot}/ThirdLibary/src/${LIBRARY} 作为第三方库源码根目录。
                                    例如：我只要编译 libvpx 库，则可以直接运行：
                                    `build_windows_msvc_libvpx.sh [libvpx源码根目录]`
      * build_windows_msvc_envsetup.sh:设置编译时需要的变量  
        环境变量说明：  
   msvc 工具链的环境变量可用下面方法设置：  
   先从菜单栏中起动vs2013编译命令行工具：  
   C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts  
   VS2013 x86 本机工具命令提示  
   在命令行下，启动msys。 
   `c:\MinGW\msys\1.0\msys.bat`  
   注意，msys中不要装link工具，否则会导致出错。如果有link工具，暂时把它命名成其它名称。  
   然后再进入脚本目录：`cd ${RabbitImRoot}/ThirdLibary/build_script`。再运行你想运行的编译脚本。例如： `./build_windows_mscv.sh` 。
   2. )mingw 工具链
      * build_windows_mingw.sh:编译所有第三依赖库
      * build_windows_mingw_${LIBRARY}: 编译具体的第三方库。它可以带一个指定第三方库源码根目录作为输入参数。如果不指定这个参数，
                                    那么将会取 ${RabbitImRoot}/ThirdLibary/src/${LIBRARY} 作为第三方库源码根目录。
                                    例如：我只要编译 libvpx 库，则可以直接运行：
                                    `build_windows_mingw_libvpx.sh [libvpx源码根目录]`
      * build_windows_mingw_envsetup.sh:设置编译时需要的变量

3. windows phone 平台：

#### mingw工具链编译：
第三库编译脚本参见：${RabbitImRoot}/ThirdLibary/build_script  
设置环境变量：

    export RabbitImRoot=/home/rabbitim    #本工程源码根目录

如果要编译 android 平台的库,还需要下面环境变量:

    export ANDROID_NDK_ROOT=     #android ndk 根目录
    export ANDROID_NDK=$ANDROID_NDK_ROOT
    export ANDROID_SDK=    #android sdk 根目录
    export ANDROID_SDK_ROOT=$ANDROID_SDK    
    export JAVA_HOME=         #jdk根目录
    export ANT_ROOT=          #ant工具的目录
    export PATH=$PATH:%ANT_ROOT/bin

可以把上面环境变量保存到  ~/.profile 文件中。  
如果编译 android 平台的库，也可以把上面环境变量保存在：
${RabbitImRoot}/ThirdLibary/build_script/build_android_envsetup.sh 中。
可以运行 ${RabbitImRoot}/ThirdLibary/build_script/build_android.sh 进行第三方依赖库编译。

1. 编解码库(libvpx)编译：  
详见：http://blog.csdn.net/kl222/article/details/23101115  
安装装到：

        make PREFIX=${RabbitImRoot}/ThirdLiabary/${Platform}

2. ffmpeg编译：  
详见《ffmpeg教程》

3. opencv编译：  
详见:http://blog.csdn.net/kl222/article/details/27223791  
编译完成后,安装到:${RabbitImRoot}/ThirdLiabary/${Platform}  

4. libspeex、libspeexdsp编译:

       ./autogen.sh  
       configure --prefix=${RabbitImRoot}/ThirdLiabary/${Platform}  
       make install  

5. webrtc编译：
详见[《webrtc教程》](http://blog.csdn.net/kl222/article/details/17198873)

6. libyuv 编译：
官网：https://code.google.com/p/libyuv/wiki/GettingStarted  
详见：[《libyuv编译》](http://blog.csdn.net/kl222/article/details/41309541)  
或者： [libyuv编译.txt](./Books/libyuv编译.txt)

7. [openssl编译](http://blog.csdn.net/kl222/article/details/41787529)

8. [libcurl 编译](http://blog.csdn.net/kl222/article/details/41898025)

9. QXMPP编译：  
    1)用 Qt Creator 进行编译（详见源码根目录下的README文件）：
    * 用 Qt Creator 打开本工程。
    * 打开“文件->打开文件或项目”。
    * 在弹出的对话框中选中qxmpp.pro，打开qxmpp工程。
    * 点左边工具栏中的“项目”，选择qxmpp标签，在相应平台“构建套件”中修改“构建步骤”参数，
           在“构建步骤”中的“额外参数”中，加入 “PREFIX=$(RabbitImRoot)/ThirdLibary/${Platform}”，
           其中$(RabbitImRoot)是本项目源码的根目录，在下面的“构建环境”变量中添加这个环境变量。
           当然，也可以直接在“额外参数”中把$(RabbitImRoot)替换成本项目源码根目录路径。
           ${Platform}为相应的平台，可以为windows、android、unix、ios。
           如果需要编译成静态库，需要在额外参数中加入：
           QXMPP_LIBRARY_TYPE=staticlib 。在本项目编译时连接静态 qxmpp 库需要增加 -DQXMPP_STATIC 。
    * 设置编解码器：现在QXMPP支持vpx、THEORA视频编解码器；G711u、SPEEX音频编解码器。音频默认为G711u。
           视频无默认编解码器，所以如果需要视频，必须指定第三方视频编解码器。
           以libvpx为例：在额外参数中填入QXMPP_USE_VPX=1
           并且添加libvpx库位置:INCLUDEPATH+=$(RabbitImRoot)/ThirdLibary/${Platform}/include
           LIBS+=-L$(RabbitImRoot)/ThirdLibary/${Platform}/lib
    * 选择windows或linux平台，在相应平台“构建套件”中的“运行”标签，
           部署->详情->部署->添加部署步骤->选择make命令->Make参数中加上"install"。
           其它平台可能会有平台自己的部署步骤，所以不能在部署这里安装。可以用下面方法：
           在相应平台“构建套件”中的“构建”标签，“构建步骤”->“make”->“make参数”中加上install。
           如果修改了代码，这种方法需要重编译。
    * 在“项目”->本项目中的“依赖关系”标签中选中qxmpp。
    * 在项目浏览器中选中qxmpp项目，右键点击“执行qmake”；再右键点击“构建”；再右键点击“部署”。
           在部署时会出现":-1: error: [install_htmldocs] Error 4 (ignored)"错误。
           这是由于没有安装doxygen，所以不能产生帮助文档。可以忽略。
    * 当前版本有BUG，需要打下面补丁
        ThirdLibary/patch/0001-add-handle-non-sasl-authentication-error-response.patch  
    2)用命令行编译：  
     * 设置 qmake 路径到环境变量 PATH 中：`export PATH=$PATH:$QMAKE_PATH`  
     * 建立编译目录：`mkdir build; cd build`
     * 生成工程文：`qmake -o Makefile QXMPP_LIBRARY_TYPE=staticlib \
                 PREFIX=$(RabbitImRoot)/ThirdLibary/${Platform} \
                 INCLUDEPATH+=$(RabbitImRoot)/ThirdLibary/${Platform}/include \
                 LIBS+=-L$(RabbitImRoot)/ThirdLibary/${Platform}/lib \
                 QXMPP_USE_VPX=1 \
                 ${RabbitImRoot}/Rabbitim.pro`
     * 编译：`$(MAKE)`  
        MAKE在不同的环境下有不同的命令：
        + make：unix、linux gun环境下用
        + nmake：msvc环境下用
        + mingw32-make：mingw 环境下用
        + msys-make：msys 环境下用

#### msvs工具链：

##### 依赖:
  * msvs
  * sh环境(msys或cygwin)
    先从菜单栏中起动vs2013编译命令行工具：  
    C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts  
    VS2013 x86 本机工具命令提示  
    在命令行下，启动msys。 
    `c:\MinGW\msys\1.0\bin\sh.exe --login -i`  
    注意，msys中不要装link工具，否则会导致出错。如果有link工具，暂时把它命名成其它名称。

#### unix 或 linux 编译（以ubuntu为例）：

##### 从源码编译参见 [mingw工具链编译] ("mingw工具链编译")

##### ubuntu中直接安装相关的库  

1. 编解码库(libvpx)安装：

    sudo apt-get install libvpx-dev

2. ffmpeg安装：

    sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavfilter-dev

3. opencv安装：

    sudo apt-get install libopencv-dev

    安装opencv时，会自动安装ffmpeg库
    
4. libspeex 安装:

    sudo apt-get install libspeex-dev libspeexdsp-dev 

5. QXMPP安装：

    sudo apt-get install libqxmpp-dev

6. webrtc编译：

    详见[《webrtc教程》](http://blog.csdn.net/kl222/article/details/17198873)

7. openssl:

    sudo apt-get install libssl-dev 

8. libcurl:

     sudo apt-get install libcurl4-openssl-dev

### 本工程编译
如果要连接静态 QXMPP 库时，需要加上-DQXMPP_STATIC 。
#### 1. 用Qt Creator 编译 

##### 1.1. 用 Qt Creator 编译 Rabbitim.pro
用 Qt Creator 打开本工程根目录下的 Rabbitim.pro 文件。
  * 下载(http://pan.baidu.com/s/1ntA0t5n )或自己编译第三方库，并放到 $(RabbitImRoot)/ThirdLibary 目录下。
  * 工具->外部->Qt 语言家->发布翻译(lrelease)，生成 *.pm 文件。
  * 把 pm 文件复制到安装目录的 translate 目录下。
  * 打开菜单：文件->打开文件，选中 Rabbitim.pro 文件
  * 构建->构建项目"RabbitIm"。编译本项目。

##### 1.2. 用 Qt Createor 和 CMake 编译 CMakeLists.txt

###### 1.2.1. windows、linux平台
用 Qt Creator 打开本工程根目录下的 CMakeLists.txt 文件。
  * 打开菜单：文件->打开文件，选中 CMakeLists.txt 文件
  * 会弹出一个执行 CMake 对话框
    + 如果是调试,在参数中填入:-DCMAKE_BUILD_TYPE=Debug 
    + 如果是发行,在参数中填入:-DCMAKE_BUILD_TYPE=Release 
    + 其它可选参数：
    -DRABBITIM_USER_LIBCURL=ON -DRABBITIM_USER_OPENSSL=ON
  * 选择相应的创建器
  * 点执行 CMake 按钮,开始执行 CMake 。如果成功，点完成就会打开项目。
  * 点调试，就可以编译，并调试程序
  * 如果要分发，选择 项目->构建->构建步骤->目标,选择 Deployment 项.

###### 1.2.2. android平台
用 Qt Creator 打开本工程根目录下的 CMakeLists.txt 文件。
  * 选择项目-> 相应平台的构建套件
  * 会弹出一个执行 CMake 对话框
    + 如果是调试,在参数中填入:-DCMAKE_BUILD_TYPE=Debug
    + 如果是发行,在参数中填入:-DCMAKE_BUILD_TYPE=Release
    + 其它可选参数：
    -DRABBITIM_USER_LIBCURL=ON -DRABBITIM_USER_OPENSSL=ON
    + 还要填入编译器参数：-DCMAKE_TOOLCHAIN_FILE=${RabbitImRoot}/platforms/android/android.toolchain.cmake -DQt5_DIR=${Qt5_DIR}
    ${Qt5_DIR}:qt for android 的 cmake 安装路径。例如：/c/Qt/Qt5.3.1_android/5.3/android_armv7/lib/cmake/Qt5
  * 选择相应的创建器
  * 点执行 CMake 按钮,开始执行 CMake 。如果成功就会打开项目。

#### 2. 用命令行编译

设置 qmake 路径到环境变量 PATH 中：`export PATH=$PATH:$QMAKE_PATH`

    mkdir build
    cd build
    qmake ${RabbitImRoot}/Rabbitim.pro
    $(MAKE)

MAKE在不同的环境下有不同的命令：  
    * make：unix、linux gun环境下用  
    * nmake：msvc环境下用  
    * mingw32-make：mingw 环境下用  
    * msys-make：msys 环境下用  

##### 2.1. 用 CMake 编译
###### 2.1.1. windows、linux平台

    mkdir rabbitim-build  #建立编译目录
    cd rabbitim-build     #进入编译目录
    cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DQt5_DIR=${Qt5_DIR}    #执行 camke
    cmake --build .       #执行编译
    ./RabbitIm            #启动程序

    Qt5_DIR:qt cmake 的安装路径。例如： /c/Qt/Qt5.3.1_android/5.3/mingw482_32/lib/cmake/Qt5

###### 2.1.2. 分发

    set PATH=%PATH$;Qt5_DIR
    cmake --build . --target Deployment
    

###### 2.1.3. android 平台:

windows 平台下以 mingw 环境为例

2.1.2.1. 先设置环境变量：


    export ANDROID_NDK_ROOT=     #android ndk 根目录
    export ANDROID_NDK=$ANDROID_NDK_ROOT
    export ANDROID_SDK=    #android sdk 根目录
    export ANDROID_SDK_ROOT=$ANDROID_SDK    
    export JAVA_HOME=         #jdk根目录
    export ANT_ROOT=          #ant工具的目录
    export PATH=$PATH:%ANT_ROOT/bin


2.1.2.2. 建立编译目录：


    mkdir rabbitim-build  #建立编译目录
    cd rabbitim-build     #进入编译目录
    

2.1.2.3. 配置、编译：


    cmake .. -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../platforms/android/android.toolchain.cmake \
        -DCMAKE_MAKE_PROGRAM=${ANDROID_MAKE} \  #windows 下需要 make 工具的位置,linux下则不需要这个
        -DQt5_DIR=${Qt5_DIR}         \   #qt 的 cmake 位置
        -DANT=${ANT_ROOT}/bin/ant    \   #ant 工具位置
    cmake --build .       #执行编译

变量说明：  

    ${ANDROID_MAKE}:windows下android ndk 自带的make工具。例如："$ANDROID_NDK/prebuilt/windows/bin/make"
    ${Qt5_DIR}:qt for android 的 cmake 安装路径。例如：Qt5_DIR=/c/Qt/Qt5.3.1_android/5.3/android_armv7/lib/cmake/Qt5
    ${ANT_ROOT}:ant 的安装路径。例如：ANT_ROOT=/d/software/apache-ant-1.9.4

例如：

    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../rabbitim/platforms/android/android.toolchain.cmake -DQt5_DIR=/c/Qt/Qt5.3.1_android/5.3/android_armv7/lib/cmake/Qt5 ../rabbitim -DANT=/d/software/apache-ant-1.9.4/bin/ant -DCMAKE_MAKE_PROGRAM=/d/software/android-ndk-r9/prebuilt/windows/bin/make -G"Unix Makefiles"

2.1.2.4. 可以会出现下面错误：

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
  c:/Qt/Qt5.3.1/5.3/android_armv7/lib/cmake/Qt5Gui/Qt5GuiConfigExtras.cmake:31 (
_qt5_Gui_check_file_exists)
  c:/Qt/Qt5.3.1/5.3/android_armv7/lib/cmake/Qt5Gui/Qt5GuiConfigExtras.cmake:58 (
_qt5gui_find_extra_libs)
  c:/Qt/Qt5.3.1/5.3/android_armv7/lib/cmake/Qt5Gui/Qt5GuiConfig.cmake:143 (inclu
de)
  c:/Qt/Qt5.3.1/5.3/android_armv7/lib/cmake/Qt5/Qt5Config.cmake:26 (find_package
)


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
