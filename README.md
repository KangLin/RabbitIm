玉兔即时通信

作者：康林（msn、email:kl222@126.com；QQ:16614119)
博客：blog.csdn.net/kl222
项目位置：https://code.csdn.net/kl222/rabbitim

玉兔即时通信是一款跨操作系统平台（Windows、Linux、Android、MacOs、IOS）的文本、语音、视频的即时通信软件。

===============================================================================================
依赖：
UI：QT
即时通信协议（XMPP）：QXMPP
视频通信：webrtc
编解码库：ffmpeg、x264、libvpx

===============================================================================================
下载工具和依赖：
QT：
主页：http://qt-project.org/
下载：http://qt-project.org/downloads
当前使用版本：5.2.1
QT开发工具参考：
  http://qt-project.org/doc/qt-4.8/developing-with-qt.html

XMPP协议（QXMPP）：
主页：http://code.google.com/p/qxmpp
下载：git clone  https://github.com/qxmpp-project/qxmpp.git
当前使用版本：
  git库版本 —— b54ccca40a16b3171cfbe4693c8970f8ca902d4f
编译方法参考源码根目录下的README文件。

webrtc库：
主页：http://www.webrtc.org/
下载：
  gclient config http://webrtc.googlecode.com/svn/trunk
  gclient sync --force
当前使用版本：svn库版本——r5663
编译参考：
  http://blog.csdn.net/kl222/article/details/17198873

编解码库（ffmpeg）：
主页：http://www.ffmpeg.org/
下载：
  git clone git://source.ffmpeg.org/ffmpeg.git
当前使用版本：
  git库版本 —— 3b06208a57b4954b50d6dcfc1be53b1b3af3dd4c

编解码库(x264)
下载：
  git clone git://git.videolan.org/x264.git
当前使用版本：
  git库版本 —— d6b4e63d2ed8d444b77c11b36c1d646ee5549276

编解码库(libvpx)
下载：
  git clone https://chromium.googlesource.com/webm/libvpx libvpx
当前使用版本：
  git库版本 —— 6962ea665dc968ce32dd1c03b6c162a35cf43c73

编解码库（libspeex）
下载：
  git clone http://git.xiph.org/speex.git
当前使用版本：
  git库版本 —— 78cce679a0f2d376b97e64a6d7763bce82dd06cc

音频处理库(libspeexdsp)
下载：
  git clone http://git.xiph.org/speexdsp.git
当前使用版本：
  git库版本 —— af768302c12f231e14076faac79b6313e49613a2

图像处理库(opencv)
下载：
  git clone git://github.com/Itseez/opencv.git
当前使用版本:
  git库版本 —— 7b366df8226fd692b1d5995d4b434c837924fa83

===============================================================================================
编译：
在源码根目录下建立第三方库目录：ThirdLibary
把第三方依赖库编译成功后，放到ThirdLibary目录下,ThirdLibary目录结构如下：
ThirdLibary
    ｜
    ｜-----patch
    ｜-----windows
    ｜       ｜-----include
    ｜       ｜-----lib
    ｜       ｜-----share
    ｜-----android
    ｜       ｜-----include
    ｜       ｜-----lib
    ｜------unix
    ｜       ｜-----include
    ｜       ｜-----lib
    ｜------ios
             ｜------include
             ｜------lib

-----------------------------------------------------------------------------------------------
mingw工具链编译：
编解码库(libvpx)编译：
详见：http://blog.csdn.net/kl222/article/details/23101115
安装装到：
make PREFIX=$(RabbitImRoot)/ThirdLiabary/${Platform}

ffmpeg编译：
详见《ffmpeg教程》

webrtc编译：
详见《webrtc教程》

QXMPP编译：
这里只说明如何用 Qt Creator 进行编译。其它方式编译，请详见源码根目录下的README文件。
1、用 Qt Creator 打开本工程。
2、打开“文件->打开文件或项目”。
3、在弹出的对话框中选中qxmpp.pro，打开qxmpp工程。
4、点左边工具栏中的“项目”，选择qxmpp标签，在相应平台“构建套件”中修改“构建步骤”参数，在“构建步骤”中的“额外参数”中，
   加入 “PREFIX=$(RabbitImRoot)/ThirdLiabary/${Platform}”，其中$(RabbitImRoot)是本项目源码的根目录，在下面的
   “构建环境”变量中添加这个环境变量。当然，也可以直接在“额外参数”中把$(RabbitImRoot)替换成本项目源码根目录路径。
   ${Platform}为相应的平台，可以为windows、android、linux、ios
5、设置编解码器：现在QXMPP支持vpx、THEORA视频编解码器；SPEEX音频编解码器。以libvpx为例：在额外参数中填入QXMPP_USE_VPX=1
   并且添加libvpx库位置:INCLUDEPATH+=$(RabbitImRoot)/ThirdLiabary/${Platform}/include
   LIBS+=-L$(RabbitImRoot)/ThirdLiabary/${Platform}/lib
6、选择相应平台“构建套件”中的“运行”标签，部署->详情->部署->添加部署步骤->选择make命令->Make参数中加上"install"。
7、在“项目”->本项目中的“依赖关系”标签中选中qxmpp。
8、在项目浏览器中选中qxmpp项目，右键点击“执行qmake”；再右键点击“构建”；再右键点击“部署”。
   在部署时会出现":-1: error: [install_htmldocs] Error 4 (ignored)"错误。
   这是由于没有安装doxygen，所以不能产生帮助文档。可以忽略。
9、当前版本有BUG，需要打下面补丁
ThirdLibary/patch/0001-add-handle-non-sasl-authentication-error-response.patch

-----------------------------------------------------------------------------------------------
msvs工具链：

===============================================================================================
开发：
本工程编码字符集为UTF-8

翻译文件部署：
1、工具->外部->Qt 语言家->发布翻译(lrelease)，生成 *.pm 文件。
2、在资源文件中指定 *.pm 文件，取别名
3、在main.cpp文件中加载需要的本地化资源

文档:
docs
  |
  |----Books      开发相关的资料
  |----TODO.txt   需要完成的事

调试：
xmpp服务配置域名（rabbitim.com），客户端如果没有域名，连接服务器时，则直接设置主机IP，同时设置域名。
当然，也可以修改系统hosts文件配置域名解析。这个域名实质上是xmpp应用程序用于区别不同域。

===============================================================================================
参考资料：
Qt Model/View： http://blog.csdn.net/leo115/article/details/7532677
