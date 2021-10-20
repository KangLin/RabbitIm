开发约定：
--------

1. 本工程编码字符集为UTF-8
2. 代码中不能包含中文，代码中只能包含英文，需要翻译的文本用 tr 包含。中文放在资源翻译文件中。
  详见：http://blog.csdn.net/kl222/article/details/38681303
    + 在 MainWindow::InitMenuTranslate() 中加入你新加入的语言项。  
3. 代码中的中文注释，在中文前后加两个空格(保证是双字节)，用于骗过vc编译器（因为vc工具链对UTF-8支持不全）。
    同样，java中的中文注释也需要在前后加两空格（java编译器也会出现对UTF-8的支持不全）
    或者：
    增加编译标志：/utf-8
    IF(MSVC)
        ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
        add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    ENDIF()

4. 如果你用qtcreator做编辑器。
    请在“工具->选项->文本编辑器->行为->保存时清理”中，取消清除空白;
    制表符和缩进尺寸都应设置为4
5. 本项目鼓励使用跨平台的开源第三方库。但在使用过程中需要遵守：
    1. 非强制使用此第三方库（即如果用户在没有此的第三方，不能影响本项目原有功能）。
        目前采用的是定义相关宏来隔离代码（即如果用户用此第三方库，申明此宏即可，如果用户不需要此第三方库，不申明此宏即可）。
    2. 提供此第三方库必要的开发文档。
    3. 提供此第三方库各平台自动编译的脚本。并放到 ${RabbitImRoot}/ThirdLibary/build_script 目录下。
    4. 第三方库二进制文件不需要上传代码库，每个人自己通过脚本自己编译。
6. 本程序使用下列日志功能宏打印日志：
    1. LOG_MODEL_ERROR(model, fmt, ...)
    2. LOG_MODEL_WARNING(model, fmt, ...)
    3. LOG_MODEL_DEBUG(model, fmt, ...) 
    4. LOG_MODEL_INFO(model, fmt, ...)
7. 线程要用setObjectName设置名称，便于调试。
8. 程序中新建立的堆对象，用智能指针代替普通指针。
9. 源码提交，基本原则为：
    1. 按每个小功能进行提交，写清楚提交注释，并保证能够编译通过。
    2. 本项目支持 CMAKE 项目文件，所以需要保证你改的功能能够编译通过。
    3. 提交前需要作者完成单元测试。
  
开发流程
--------

### 开发者流程

1. 开发者是指现在玉兔即时通讯的开发人员，或者以后我们信任的贡献者转化而成的开发人员。
2. 要成为开发者，需要在 github 上注册账号, 然后由管理者加入到相应项目的 collaborators 列表
3. 开发主要用到 master 和 Develop 两个分支， 平时开发都在 Develop 分支上，只有代码
    达到一个 milestone 的 stable 状态，才把 Develop 分支合并到 master 分支
4. 有时开发者可能想实现一个比较 cool 的功能，可以建立一个 feature_x 分支，
    测试稳定后再合并到 master 分支
5. 有好的主意或发现BUG、讨论等，请到 https://github.com/KangLin/rabbitim/issues

### 贡献者流程

1. 贡献者是指非玉兔即时通讯项目组成员，热爱开源且希望为开源项目贡献代码的开发人员
2. 贡献者可以在 github 上 Fork 一个子项目，然后在 Fork 的项目上提交代码，
    再通过 Pull Request 把修改通知给项目开发者（**注意** pull Request 前必须先合并当前 master 到本地，解决所有可能的冲突），
    由开发者审查代码后，决定是否合并进入 master 分支， 具体可参考: 
    [github协作流程](http://www.worldhello.net/gotgithub/04-work-with-others/010-fork-and-pull.html)
3. 有好的主意或发现BUG、讨论等，请到 https://github.com/KangLin/rabbitim/issues

### 版本迭代流程
1. 版本迭代周期暂定为3个月
2. 开发者和贡献者可以把想要实现的功能通过 github 的 wiki 功能提交上来
3. 开始迭代前讨论本期版本要实现哪些功能，然后把要在本次迭代实现的功能列表写入版本的[TODO feature list列表](TODO.txt)
4. 制定大概的排期
5. 开发，内部测试
6. alpha版本发布，公测
7. 把 Develop 分支代码 merge 到 master 分支，stable 版本发布

        git checkout master #切换到Master分支
        git merge Develop   #对Develop分支进行合并

8. 打tag

        git tag -a vX.X.X -m 'Release version X.X.X

将本地tag　push到服务器：

        git push origin vX.X.X

### 版本号约定

[语义化版本](http://semver.org/)

* 本项支持 CMake 项目文件，所以需要**同时修改以下文件中的版本号**：
    1. 用cmake 发行时需要修改：[install_version.cmake](./../cmake/install_version.cmake)
    2. 修改自动化编译配置文件：[appveyor.yml](../appveyor.yml)

#### 格式:
1. 格式：  
    Major_Version_Number.Minor_Version_Number[.Revision_Number[.Build_Number[.Build_TIME[.Suffix]]]]  
    主版本号.子版本号[.修正版本号[.编译版本号[.编译时间[.后缀]]]]
2. 示例 :
    * 2.0
    * 2.2.1
    * 5.0.0.13124
3. 参数说明：
    + Major ：具有相同名称但不同主版本号的程序集不可互换。例如，这适用于对产品的大量重写，这些重写使得无法实现向后兼容性。
    + Minor ：如果两个程序集的名称和主版本号相同，而次版本号不同，这指示显著增强，但照顾到了向后兼容性。
        例如，这适用于产品的修正版或完全向后兼容的新版本。
        **本项目中偶数表示稳定版本，奇数表示测试版本**
    + Revision ：名称、主版本号和次版本号都相同但修订号不同的程序集应是完全可互换的。这适用于修复以前发布的程序集中的安全漏洞。
        **本项目中，如果Minor为奇数，则0表示α（alphal） 内部测试版；1表示β（beta）外部测试版 ;3表示γ（gamma）版**
    + Build ：编译时编码在git代码库中的版本号
    + Build_TIME:编译的时间
    + Suffix:后缀,用于说明当前版本
        * α（alphal） 内部测试版：
            α版，此版本表示该软件仅仅是一个初步完成品，通常只在软件开发者内部交流，也有很少一部分发布给专业测试人员。
            一般而言，该版本软件的 bug 较多，普通用户最好不要安装。 
        * β（beta）外部测试版：
            该版本相对于α版已有了很大的改进，消除了严重的错误，但还是存在着一些缺陷，需要经过大规模的发布测试来进一步消除。
            这一版本通常由软件公司免费发布，用户可从相关的站点下载。
            通过一些专业爱好者的测试，将结果反馈给开发者，开发者们再进行有针对性的修改。该版本也不适合一般用户安装。
        * γ（gamma）版：
            该版本已经相当成熟了，与即将发行的正式版相差无几，如果用户实在等不及了，尽可以装上一试。
        * trial（试用版）：
            试用版软件在最近的几年里颇为流行，主要是得益于互联网的迅速发展。
        * release 最终释放版：
            该版本意味“最终释放版”，在出了一系列的测试版之后，终归会有一个正式版本，
            该版本有时也称为标准版。一般情况下，release不会以单词形式出现在软件封面上，取而代之的是符号 (r) ，
            如 windows nt(r) 4.0、ms-dos(r) 6.22 等。
        * standard 标准版：
            这是最常见的标准版，不论是什么软件，标准版一定存在。
            标准版中包含了该软件的基本组件及一些常用功能，可以满足一般用户的需求。
        * professional（专业版）：
            专业版是针对某些特定的开发工具软件而言的。
            专业版中有许多内容是标准版中所没有的，这些内容对于一个专业的软件开发人员来说是极为重要的。
        * enterprise（企业版）：
            企业版是开发类软件中的极品（相当于百科全书中的reference版）。

#### 管理策略：
1. 当项目在进行了局部修改或 bug 修正时，主版本号和子版本号都不变，修正版本号加 1；
2. 子版本号奇数表示为测试版本，偶数表示为稳定版本；
3. 当项目在原有的基础上增加了部分功能时，主版本号不变，子版本号加 1，修正版本号复位为 0，因而可以被忽略掉；
4. 当项目在进行了重大修改或局部修正累积较多，而导致项目整体发生全局变化时，主版本号加 1；
5. 另外，编译版本号一般是编译器在编译过程中自动生成的，我们只定义其格式，并不进行人为控制。本项目为GIT库中的版本号。

#### 检查列表
完成开发后，提交前，需要检查下面事项是否完成：

##### 1. 代码
1. 代码中的中文注释后是否加有两个空格
2. 代码中的字符串必须是英文
3. 是否已修改相关翻译文件
4. 是否已修改相关 cmake 工程文件
5. 是否完成相关文档(TODO.txt,ChangeLog.md)
6. 是否完成了第三方库各平台编译脚本

##### 2. 编译
1. WINDOWS for msvc 编译器是否能通过（已自动化）
2. windows for mingw 编译器是否能通过（已自动化）
3. linux for g++ 编译器是否能通过（已自动化）
4. android 编译器是否能通过（已自动化）
5. 用 CMake 工程是否能通过（已自动化）

##### 3. 是否修改了版本号（仅管理员检查此项）
1. 用cmake 发行时需要修改：[install_version.cmake](./../cmake/install_version.cmake)
2. 修改自动化编译配置文件：[appveyor.yml](../appveyor.yml)

参考资料：
--------

1. [开发笔记](./Books/开发笔记.md)
2. [安装](INSTALL.md)
