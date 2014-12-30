1. 修改工程文件 .pro ，加入翻译源文件 hello_world.ts：


    TRANSLATIONS += \
        Resource/translations/hello_world.ts


写代码时需要注意下面两点：  
2. 源码用 utf-8 字符集
3. 源码中需要翻译的字符串必须用英文(不能有中文或其它语言），则需要用 tr()  函数。 
例如：

    QMessageBox::information(this,tr("Information"),tr("Hello world."));

4.更新翻译源文件： 
工具-》外部-》qt 语言家-》更新翻译（lupdtae） 
工具 lupdate﻿﻿ 自动分析工程文件 .pro ，找到翻译源文件 hello_world.ts ；然后会分析源码中包含在 tr 中的字符串，写入到翻译源文件 hello_world.ts 中。翻译源文件是一个 xml 文件。

 <context>
     <name>Hello</name>
     <message>
  <location filename="HelloWorld.ui" line="14"/>
  <source>Frame</source>
  <translation type="unfinished"></translation>
     </message>
 </context>

5、修改翻译源文件 hello_world.ts ： 
5.1. 你可以直接编辑翻译源文件进行翻译
5.2. 你也可以用工具 linguist 进行翻译

6. 在代码中加载需要的翻译资源文件 
6.1. 把翻译资源文件放到程序的资源文件中 
把翻译资源文件放到程序的资源文件中后，就会被当做程序的资源编译进程序中。所以访问翻译资源文件的方式与其它资源的访问方式相同。即：“:/translat/hello_world.qm” 。
用“:/”来指定是从资源中加载。
例如：

 //本地化程序资源 
 QTranslator myappTranslator;
 //把翻译文件放在了程序资源中 
 myappTranslator.load(":/translations/hello_world.qm");
 qApp->installTranslator(&myappTranslator);


6.2. 从翻译资源文件中直接加载

 //本地化程序资源 
 QTranslator myappTranslator;
 //把翻译文件放在了程序资源中 
 myappTranslator.load("C:/translations/hello_world.qm");
 qApp->installTranslator(&myappTranslator);


7. 编译：
工具-》外部-》qt 语言家-》更新翻译（lrelease） 
翻译完翻译源文件后，需要翻译编译工具（lrelease）把翻译源文件编译成翻译资源文件 hello_world.qm 。

8. qt 资源：
qt 翻译资源在安装目录下的 translate 目录下。

9. 在程序中做翻译菜单：
当语言进行切换时，需要调用 ui->retranslateUi(this); 更新主窗口。 
如果非主窗口，则这个函数会触发 void changeEvent(QEvent *e) 事件。
例如：

 void CHelloWorld::changeEvent(QEvent *e)
 {
     LOG_MODEL_DEBUG("MainWindow", "MainWindow::changeEvent.e->type:%d", e->type());
     switch(e->type())
     {
     case QEvent::LanguageChange:
  ui->retranslateUi(this);
  break;
     }
 }