开发约定：
--------

* 本工程编码字符集为UTF-8

* 代码中不能包含中文，代码中只能包含英文，需要翻译的文本用 tr 包含。中文放在资源翻译文件中。  
  详见：http://blog.csdn.net/kl222/article/details/38681303  
  + 在 MainWindow::InitMenuTranslate() 中加入你新加入的语言项。  

* 代码中的中文注释，在中文前后加两个空格(保证是双字节)，用于骗过vc编译器（因为vc工具链对UTF-8支持不全）。

* 如果你用qtcreator做编辑器，请在“工具->选项->文本编辑器->行为->保存时清理”中，取消清除空白。  
  制表符和缩进尺寸都应设置为4

* 本项目鼓励使用跨平台的开源第三方库。  
  但在使用过程中需要遵守：
  1. 非强制使用此第三方库（即如果用户在没有此的第三方，不能影响本项目原有功能）。  
     目前采用的是定义相关宏来隔离代码（即如果用户用此第三方库，申明此宏即可，如果用户不需要此第三方库，不申明此宏即可）。
  2. 提供此第三方库必要的开发文档。
  3. 提供此第三方库各平台自动编译的脚本。并放到 ${RabbitImRoot}/ThirdLibary/build_script 目录下。
  4. 第三方库二进制文件不需要上传代码库，每个人自己通过脚本自己编译。

* 本程序使用下列日志功能宏打印日志：
  1. LOG_MODEL_ERROR(model, fmt, ...)
  2. LOG_MODEL_WARNING(model, fmt, ...)
  3. LOG_MODEL_DEBUG(model, fmt, ...) 
  4. LOG_MODEL_INFO(model, fmt, ...)

* 线程要用setObjectName设置名称，便于调试。

* 程序中新建立的堆对象，用智能指针代替普通指针。

* 源码提交：  
  提交的基本原则为：
  1. 按每个小功能进行提交，写清楚提交注释，并保证能够编译通过。
  2. 提交前需要作者完成单元测试。
  
开发流程
--------

###开发者流程

- 开发者是指现在玉兔即时通信的开发人员，或者以后我们信任的贡献者转化而成的开发人员。

- 要成为开发者，需要在github上注册账号, 然后由管理者加入到相应项目的collaborators列表

- 开发主要用到master和develop两个分支， 平时开发都在develop分支上，只有代码
达到一个milestone的stable状态，才把develop分支merge到master分支

- 有时开发者可能想实现一个比较cool的feature，可以建立一个feature_x分支，
测试稳定后merge到master

###贡献者流程

- 贡献者是指非玉兔即时通信项目组成员，热爱开源且希望为开源项目贡献代码的开发人员

- 贡献者可以在github上Fork一个子项目，然后在Fork的项目上提交代码，
再通过Pull Request把修改通知给项目开发者，由开发者code review后，
决定是否merge进入master分支， 具体可参考: [github协作流程](http://www.worldhello.net/gotgithub/04-work-with-others/010-fork-and-pull.html)

###版本迭代流程
- 版本迭代周期暂定为3个月
- 开发者和贡献者可以把想要实现的feature通过github的wiki功能提交上来
- 开始迭代前讨论本期版本要实现哪些feature，然后把要在本次迭代实现的featue列表写入版本的TODO feature list列表
- 制定大概的排期
- 开发，内部测试
- alpha版本发布，公测
- 把develop分支代码merge到master分支，stable版本发布

参考资料：
--------

* [Qt Model/View](http://blog.csdn.net/leo115/article/details/7532677)

* [README.md语法格式](https://github.com/LearnShare/Learning-Markdown/blob/master/README.md)

* [Qt之文本编辑](http://blog.chinaunix.net/uid-25806493-id-3346294.html)
