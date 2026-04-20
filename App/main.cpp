#include "MainWindow.h"
#include "Global/Global.h"
#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <QSsl>
#include <QSslSocket>
#include <QLoggingCategory>

#ifdef RABBITCOMMON
    #include "RabbitCommonTools.h"
    #include "RabbitCommonDir.h"
    #include "FrmUpdater/FrmUpdater.h"
    #ifdef BUILD_QUIWidget
        #include "QUIWidget/QUIWidget.h"
    #endif
#endif

#ifdef Q_OS_ANDROID
    #include <QtAndroid>
#endif

static Q_LOGGING_CATEGORY(log, "App.Main")
int main(int argc, char *argv[])
{
    int nRet = 0;

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    /* 修复使用 Qt6 时，最大化时，工具栏位置错误。
       现在很多 linux 用 wayland 或 wayland-egl 作为桌面显示，这样会出现一个问题，
       由于没有坐标系统，导致无边框窗体无法拖动和定位（即 QWidge::move 失效）。
      （Qt6 开始强制默认优先用 wayland ，Qt5 默认有 xcb 则优先用 xcb），
       所以需要在 main 函数最前面加一行 `qputenv("QT_QPA_PLATFORM", "xcb")`;
    */
    QString szPlatform = qEnvironmentVariable("QT_QPA_PLATFORM");
    if(szPlatform.isEmpty()
        || -1 != szPlatform.indexOf(QRegularExpression("wayland.*")))
        qputenv("QT_QPA_PLATFORM", "xcb");
    qInfo(log) << "QT_QPA_PLATFORM:" << szPlatform << "\nCurrent Qt Platform is:"
               << qEnvironmentVariable("QT_QPA_PLATFORM")
               << "; This can be modified with the environment variables QT_QPA_PLATFORM:\n"
               << "export QT_QPA_PLATFORM=xcb\n Optional: xcb; vnc";
#endif

#if (QT_VERSION > QT_VERSION_CHECK(5,6,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    QtAndroid::hideSplashScreen();
#endif

    QApplication::setApplicationVersion(RabbitIm_VERSION);
    QApplication::setApplicationName("RabbitIm");
    QApplication::setApplicationDisplayName(QObject::tr("Rabbit Im"));
    QApplication::setOrganizationName("KangLin studio");

    QApplication app(argc, argv);

#ifdef RABBITCOMMON
    RabbitCommon::CTools::Instance()->Init();

    // Check update version
    if(qEnvironmentVariable("SNAP").isEmpty()
        && qEnvironmentVariable("FLATPAK_ID").isEmpty()) {
        QSharedPointer<CFrmUpdater> pUpdater(new CFrmUpdater());
        if(pUpdater) {
            pUpdater->SetTitle(QImage(":/icon/AppIcon"));
            if(app.arguments().length() > 1) {
                try{
                    pUpdater->GenerateUpdateJson();
                    pUpdater->GenerateUpdateXml();
                } catch(...) {
                    qCritical(log) << "Generate update fail";
                }
                qInfo(log) << app.applicationName() + " " + app.applicationVersion()
                                  + " " + QObject::tr("Generate update json file End");
                return 0;
            }
        } else {
            qCritical(log) << "new CFrmUpdater() fail";
        }
    }
#endif

    QSharedPointer<QTranslator> tApp =
        RabbitCommon::CTools::Instance()->InstallTranslator("RabbitImApp");

    app.setApplicationDisplayName(QObject::tr("Rabbit immediate communicate"));

    //QFontDatabase::addApplicationFont("://DejaVuSans.ttf");
    //a.setFont(QFont(DejaVuSans));
    qDebug(log, "font:%s", app.font().family().toStdString().c_str());

    //设置插件路径(msvc 下没有用）   
    app.addLibraryPath(RabbitCommon::CDir::Instance()->GetDirApplication());
    app.addLibraryPath(RabbitCommon::CDir::Instance()->GetDirApplicationInstallRoot()
                     + QDir::separator() + "plugins");

    CManager manager;
    CGlobal::Instance()->SetManager(&manager);

    //*
    MainWindow* w = new MainWindow();

    try {
#ifndef MOBILE
        //加载窗口位置  
        QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                       QSettings::IniFormat);
        QScreen *pScreen = QGuiApplication::primaryScreen();
        
        int top = conf.value("UI/MainWindow/top",
                             pScreen->availableGeometry().height()  > w->height() 
                             ? (pScreen->availableGeometry().height() - w->height()) >> 1
                             :  60
                               ).toInt();
        int left = conf.value("UI/MainWindow/left",
                              pScreen->availableGeometry().width() > w->width() 
                              ? (pScreen->availableGeometry().width() - w->width()) >> 1
                              : 60
                                ).toInt();
        int Width = conf.value("UI/MainWindow/width", 
                               pScreen->availableGeometry().width() > w->width() 
                               ? w->geometry().width()
                               : pScreen->availableGeometry().width()  - 120
                                 ).toInt();
        int Height = conf.value("UI/MainWindow/height",
                                pScreen->availableGeometry().height()  > w->height() 
                                ? w->geometry().height()
                                :  pScreen->availableGeometry().height()  - 120
                                  ).toInt();
#endif

#ifndef MOBILE
        w->resize(Width, Height);
        w->move(left, top);
#endif
        w->show();

        //*/

        /*以下为视频捕获、显示测试代码(CFrmPlayer::TestCamera())  
#ifdef DEBUG
    CFrmPlayer player;
    player.TestCamera();
    player.show();
#endif
    //*/
        nRet = app.exec();
    } catch(std::exception &e) {
        qCritical("exception: %s", e.what());
    } catch(...) {
        qCritical("exception");
    }

#ifndef BUILD_QUIWidget
    delete w;
#endif

    if(tApp)
        RabbitCommon::CTools::Instance()->RemoveTranslator(tApp);

#ifdef RABBITCOMMON
    RabbitCommon::CTools::Instance()->Clean();
#endif

    return nRet;
}
