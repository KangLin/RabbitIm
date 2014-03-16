玉兔即时通信

作者：康林（email:kl222@126.com、QQ:16614119)
博客：blog.csdn.net/kl222
项目位置：https://code.csdn.net/kl222/rabbitim

依赖：
UI：QT
即时通信协议（XMPP）：QXMPP
视频通信：webrtc

下载工具和依赖：
QT：
主页：http://qt-project.org/
下载：http://qt-project.org/downloads
当前使用版本：5.2.1
QT开发工具参考：
  http://qt-project.org/doc/qt-4.8/developing-with-qt.html

XMPP协议：
主页：http://code.google.com/p/qxmpp
下载：git clone  https://code.google.com/p/qxmpp/
当前使用版本：
  git库版本——a4c3f19e874cace34af4476a06cc4b530b46c516
编译方法参考源码根目录下的README文件。

webrtc：
主页：http://www.webrtc.org/
下载：
  gclient config http://webrtc.googlecode.com/svn/trunk
  gclient sync --force
当前使用版本：svn库版本——r5663
编译参考：
  http://blog.csdn.net/kl222/article/details/17198873

编译：
在源码根目录下建立第三方库目录：ThirdLibary
把第三方依赖库编译成功后，放到ThirdLibary目录下,ThirdLibary目录结构如下：
ThirdLibary
    ｜
    ｜-----include
    ｜-----lib
    ｜-----share

QXMPP编译：
这里只说明如何用 Qt Creator 进行编译。其它方式编译，请详见源码根目录下的README文件。
1、用 Qt Creator 打开本工程。
2、打开“文件->打开文件或项目”。
3、在弹出的对话框中选中qxmpp.pro，打开qxmpp工程。
4、点左边工具栏中的“项目”，选择qxmpp标签，在相应平台“构建套件”中修改“构建步骤”参数，在“构建步骤”中的“额外参数”中，
加入 “PREFIX=$(RabbitImRoot)/ThirdLiabary”，其中$(RabbitImRoot)是本项目源码的根目录，在下面的
“构建环境”变量中添加这个环境变量。当然，也可以直接在“额外参数”中替换成本项目源码根目录路径。
5、选择相应平台“构建套件”中的“运行”标签，部署->详情->Make参数中加上"install"。
6、在“项目”->本项目中的“依赖关系”标签中选中qxmpp。
6、在项目浏览器中选中qxmpp项目，右键点击“执行qmake”；再右键点击“构建”；再右键点击“部署”。
7、在部署时会出现":-1: error: [install_htmldocs] Error 4 (ignored)"错误。
这是由于没有安装doxygen，所以不能产生帮助文档。可以忽略。
8、当前版本有BUG，需要打下面补丁
ThirdLibary/patch/0001-add-handle-non-sasl-authentication-error-response.patch


翻译文件部署：
1、工具->外部->Qt 语言家->发布翻译(lrelease)，生成 *.pm 文件。
2、手工把源码根目录下的 translations/app_*.pm 复制到执行程序的相同目录下。

