#include "MainWindow.h"
#include "Widgets/FrmVideo/FrmPlayer.h"
#include "Global/Global.h"
#include "common/Tool.h"
#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <QSsl>
#include <QSslSocket>

#ifdef RABBITCOMMON
    #include "RabbitCommonTools.h"
    #include "RabbitCommonDir.h"
    #include "FrmUpdater/FrmUpdater.h"
#endif

int main(int argc, char *argv[])
{
#if (QT_VERSION > QT_VERSION_CHECK(5,6,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
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

    if(!QSslSocket::supportsSsl())
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 3))
        qCritical() << "Please install openssl first. openssl build version:"
                    << QSslSocket::sslLibraryBuildVersionString();
#endif
    } else {
        qDebug() << "Support ssl:" << QSslSocket::supportsSsl()
            #if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 3))
                << "sslLibraryBuildVersionString:"
                << QSslSocket::sslLibraryBuildVersionString()
           #endif
                << "sslLibraryVersionString:" 
                << QSslSocket::sslLibraryVersionString();
    }
    
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
    
#ifdef RABBITIM_USE_FFMPEG
    CTool::SetFFmpegLog();
#endif
    
    //*
    MainWindow w;
#ifndef MOBILE
    //加载窗口位置  
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    QScreen *pScreen = QGuiApplication::primaryScreen();
    
    int top = conf.value("UI/MainWindow/top",
          pScreen->availableGeometry().height()  > w.height() 
          ? (pScreen->availableGeometry().height() - w.height()) >> 1
          :  60
          ).toInt();
    int left = conf.value("UI/MainWindow/left",
          pScreen->availableGeometry().width() > w.width() 
          ? (pScreen->availableGeometry().width() - w.width()) >> 1
          : 60
          ).toInt();
    int Width = conf.value("UI/MainWindow/width", 
          pScreen->availableGeometry().width() > w.width() 
          ? w.geometry().width()
          : pScreen->availableGeometry().width()  - 120
          ).toInt();
    int Height = conf.value("UI/MainWindow/height",
          pScreen->availableGeometry().height()  > w.height() 
          ? w.geometry().height()
          :  pScreen->availableGeometry().height()  - 120
          ).toInt();
    w.resize(Width, Height);
    w.move(left, top);
#endif
    w.show();

    //*/

    /*以下为视频捕获、显示测试代码(CFrmPlayer::TestCamera())  
#ifdef DEBUG
    CFrmPlayer player;
    player.TestCamera();
    player.show();
#endif
    //*/
    int nRet = app.exec();

#ifdef DEBUG
    Q_CLEANUP_RESOURCE(translations_RabbitImApp);
#endif

    return nRet;
}
