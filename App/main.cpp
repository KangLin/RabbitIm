#include "MainWindow.h"
#include "Global/Global.h"
#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <QSsl>
#include <QSslSocket>

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

int main(int argc, char *argv[])
{
    int nRet = 0;
#if (QT_VERSION > QT_VERSION_CHECK(5,6,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    QtAndroid::hideSplashScreen();
#endif
//#ifdef ANDROID
//    Q_INIT_RESOURCE(Android);
//#endif
#if defined(ANDROID) || defined(RABBITIM_STATIC)
    Q_INIT_RESOURCE(Resource);
    //Q_INIT_RESOURCE(style);

#endif

#ifdef DEBUG
    Q_INIT_RESOURCE(translations_RabbitImApp);
#endif

    QApplication app(argc, argv);
    app.setApplicationVersion(BUILD_VERSION);
    
    app.setApplicationName("RabbitIm");
    app.setOrganizationName("KangLin studio");

#ifdef RABBITCOMMON
    RabbitCommon::CTools::Instance()->Init();
    CFrmUpdater *pUpdate = new CFrmUpdater();
    pUpdate->SetTitle(QImage(":/icon/AppIcon"));
    if(!pUpdate->GenerateUpdateXml())
        return 0;
#endif

    QTranslator translator;
    translator.load(RabbitCommon::CDir::Instance()->GetDirTranslations()
                    + QDir::separator() + "RabbitImApp_"
                    + QLocale::system().name() + ".qm");
    qApp->installTranslator(&translator);
    
    app.setApplicationDisplayName(QObject::tr("Rabbit immediate communicate"));

    //QFontDatabase::addApplicationFont("://DejaVuSans.ttf");
    //a.setFont(QFont(DejaVuSans));
    LOG_MODEL_DEBUG("main", "font:%s;codec:%s",
                    app.font().family().toStdString().c_str(),
                    QTextCodec::codecForLocale()->name().data());

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
#ifdef BUILD_QUIWidget
        QSharedPointer<QUIWidget> quiwidget(new QUIWidget(nullptr, true));
        //quiwidget.setPixmap(QUIWidget::Lab_Ico, ":/image/App");
        //quiwidget.setTitle(a.applicationDisplayName());
        quiwidget->setVisible(QUIWidget::BtnMenu_Max, false);
        quiwidget->setMainWidget(w);
        #ifndef MOBILE
        quiwidget->resize(Width, Height);
        quiwidget->move(left, top);
        #endif
        quiwidget->show();
#else
#ifndef MOBILE
        w->resize(Width, Height);
        w->move(left, top);
#endif
        w->show();
#endif
        
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
        LOG_MODEL_ERROR("main", "exception: %s", e.what());
    } catch(...) {
        LOG_MODEL_ERROR("main", "exception");
    }
    
#ifndef BUILD_QUIWidget
    delete w;
#endif
    
#ifdef RABBITCOMMON
    RabbitCommon::CTools::Instance()->Clean();
#endif
    app.removeTranslator(&translator);
#if defined (_DEBUG) || !defined(BUILD_SHARED_LIBS)
    Q_CLEANUP_RESOURCE(translations_RabbitImApp);
#endif

    return nRet;
}
