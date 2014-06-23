玉兔即时通信
==========

作者：康林（msn、email:kl222@126.com；QQ:16614119)  
博客：http://blog.csdn.net/kl222  
项目位置：https://code.csdn.net/kl222/rabbitim

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

功能:
-----

1. 跨平台  
    玉兔即时通信是一款跨操作系统平台（Windows、Linux、Android、MacOs、IOS、windows phone）
的文本、语音、视频的即时通信软件。
2. xmpp协议的即时通信软件
3. 支持语音、视频（p2p）
4. 支持会议室功能；语音、视频会议
5. 支持远程桌面
6. 支持白板
7. 支持文件传输

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

许可协议:
-------
1. 本软件允许个人或50人以下的企业、集体、单位免费使用。
2. 50人以上的企事业、集体、单位使用需要得到本人书面许可。
3. 未经本人书面许可，任何人不得利用本软件从事商业活动。
4. 其它未尽事宜遵守《GPL协议》

依赖：
-----

* 开发工具：git、svn、autoconf、automake、make、cmake、python、perl、bash、
* Windows 下开发环境:Mingw（或者cygwin）、msvc
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

3.mingw（或者cygwin）：
mingw主页：http://www.mingw.org/  
cygwin主页：http://www.cygwin.org/  
当前使用的是 mingw ，因为 QT for android in windows 预编译包中已包含了 mingw 。只需要下载 msys 。

3. XMPP协议（QXMPP）：

主页：http://code.google.com/p/qxmpp  
下载：
`git clone  https://github.com/qxmpp-project/qxmpp.git`  
当前使用版本：  
* git库版本 —— b54ccca40a16b3171cfbe4693c8970f8ca902d4f  

编译方法参考源码根目录下的README文件。
        
4. webrtc库：
    
主页：http://www.webrtc.org/  
下载：

    gclient config http://webrtc.googlecode.com/svn/trunk
    gclient sync --force

当前使用版本：svn库版本——r5663  
编译参考：http://blog.csdn.net/kl222/article/details/17198873

5. 编解码库（ffmpeg）：  
主页：http://www.ffmpeg.org/  
下载：

    git clone git://source.ffmpeg.org/ffmpeg.git

当前使用版本：
* git库版本 —— 3b06208a57b4954b50d6dcfc1be53b1b3af3dd4c  
        
6. 编解码库(x264)

下载：`git clone git://git.videolan.org/x264.git`  
当前使用版本：
* git库版本 —— d6b4e63d2ed8d444b77c11b36c1d646ee5549276
        
7. 解码库(libvpx)

下载：`git clone https://chromium.googlesource.com/webm/libvpx libvpx`  
当前使用版本:
* git库版本 —— 6962ea665dc968ce32dd1c03b6c162a35cf43c73
        
8. 音频处理库(libspeexdsp)

下载：`git clone http://git.xiph.org/speexdsp.git`  
当前使用版本:
* git库版本 —— af768302c12f231e14076faac79b6313e49613a2

9. 音频编解码库（libspeex）

下载：`git clone http://git.xiph.org/speex.git`  
当前使用版本：
* git库版本 —— 78cce679a0f2d376b97e64a6d7763bce82dd06cc
              
10. 图像处理库(opencv)

下载：`git clone git://github.com/Itseez/opencv.git`  
当前使用版本:
* git库版本 —— c9db91ace508b39af513d04412b3c62399e7ee1b

11. libyuv
下载:`svn checkout http://libyuv.googlecode.com/svn/trunk libyuv`

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
        ｜-----windows          # Windows 平台的第三方库
        ｜       ｜-----include #头文件
        ｜       ｜-----lib     #库文
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

#### 第三方库编译脚本说明
第三库编译脚本是 bash 脚本。运行这些脚本时，需要有 bash 环境。linux、unix 默认安装了 bash 环境。
windows 下需要安装 cygwin 或者 msys 环境。  

环境变量：

    export RabbitImRoot=/home/rabbitim    #本工程源码根目录
    
所需要的环境变量可以保存到系统配置文件 ~/.profile 文件中。作为全局环境变量。但这可能会与其它工程需要的环境变量冲突。
为了避免这个问题。你也可以把环境变量保到 build_${PLATFORM}_envsetup.sh 文件中。

脚本只编译第三方依赖库的发行版本，调试版本，请手工编译。

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

6. webrtc编译：
详见《webrtc教程》

7. libyuv 编译：
详见：https://code.google.com/p/libyuv/wiki/GettingStarted  
或者： ${RabbitImRoot}/docs/Books/libyuv编译.txt

8. QXMPP编译：  
    1)用 Qt Creator 进行编译（详见源码根目录下的README文件）：
    * 用 Qt Creator 打开本工程。
    * 打开“文件->打开文件或项目”。
    * 在弹出的对话框中选中qxmpp.pro，打开qxmpp工程。
    * 点左边工具栏中的“项目”，选择qxmpp标签，在相应平台“构建套件”中修改“构建步骤”参数，
           在“构建步骤”中的“额外参数”中，加入 “PREFIX=$(RabbitImRoot)/ThirdLiabary/${Platform}”，
           其中$(RabbitImRoot)是本项目源码的根目录，在下面的“构建环境”变量中添加这个环境变量。
           当然，也可以直接在“额外参数”中把$(RabbitImRoot)替换成本项目源码根目录路径。
           ${Platform}为相应的平台，可以为windows、android、unix、ios。
           如果需要编译成静态库，需要在额外参数中加入：
           QXMPP_LIBRARY_TYPE=staticlib  
    * 设置编解码器：现在QXMPP支持vpx、THEORA视频编解码器；G711u、SPEEX音频编解码器。音频默认为G711u。
           视频无默认编解码器，所以如果需要视频，必须指定第三方视频编解码器。
           以libvpx为例：在额外参数中填入QXMPP_USE_VPX=1
           并且添加libvpx库位置:INCLUDEPATH+=$(RabbitImRoot)/ThirdLiabary/${Platform}/include
           LIBS+=-L$(RabbitImRoot)/ThirdLiabary/${Platform}/lib
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
     * 生成工程文：`qmake ${RabbitImRoot}/Rabbitim.pro`
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

    详见《webrtc教程》

### 本工程编译

#### Qt Creator 编译

用 Qt Creator 打开本工程根目录下的 Rabbitim.pro 文件。

#### 用命令行编译

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
    
开发：
--------------------
1. 本工程编码字符集为UTF-8
2. 代码中不能包含中文，代码中包含英文，中文放在资源翻译文件中。
3. 代码中的中文注释，在中文前后加一个空格(保证是双字节)，用于骗过vc编译器（因为vc工具链对UTF-8支持不全）。
4. 如果你用qtcreator做编辑器，请在“工具->选项->文本编辑器->行为->保存时清理”
           中，取消清除空白

翻译文件部署：
--------------------
1. 工具->外部->Qt 语言家->发布翻译(lrelease)，生成 *.pm 文件。
2. 在资源文件中指定 *.pm 文件，取别名
3. 在main.cpp文件中加载需要的本地化资源

文档:
-------------------

    docs
      |
      |----Books      开发相关的资料
      |----TODO.txt   需要完成的事

调试：
-------------------

xmpp服务配置域名（rabbitim.com），客户端如果没有域名，连接服务器时，则直接设置主机IP，同时设置域名。  
当然，也可以修改系统hosts文件配置域名解析。这个域名实质上是xmpp应用程序用于区别不同域。

参考资料：
-------------------

* Qt Model/View： http://blog.csdn.net/leo115/article/details/7532677
* README.md语法格式参见:https://github.com/LearnShare/Learning-Markdown/blob/master/README.md
