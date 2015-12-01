#include "MainWindow.h"
#include "Widgets/FrmVideo/FrmPlayer.h"
#include "Global/Global.h"
#include "Tool.h"
#include <QDebug>
#include <QApplication>
#include <QTranslator>
#ifdef RABBITIM_USE_LIBCURL
#include "Update/DownLoadHandle.h"
#endif

int main(int argc, char *argv[])
{
#ifdef ANDROID
    Q_INIT_RESOURCE(Android);
#endif
#if defined(ANDROID) || defined(RABBITIM_STATIC)
    Q_INIT_RESOURCE(Resource);
    Q_INIT_RESOURCE(style);
    Q_INIT_RESOURCE(Translations);
#endif

    QApplication app(argc, argv);
    app.setApplicationName("RabbitIm");
    app.setOrganizationName("KangLin studio");

    //QFontDatabase::addApplicationFont("://DejaVuSans.ttf");
    //a.setFont(QFont(DejaVuSans));
    LOG_MODEL_DEBUG("main", "font:%s;codec:%s",
                    app.font().family().toStdString().c_str(),
                    QTextCodec::codecForLocale()->name().data());

    //设置插件路径(msvc 下没有用）   
    app.addLibraryPath(CGlobalDir::Instance()->GetDirApplication());
    app.addLibraryPath(CGlobalDir::Instance()->GetDirApplication()
                     + QDir::separator() + "plugins");

#ifdef RABBITIM_USE_FFMPEG
    CTool::SetFFmpegLog();
#endif
    
    //*
    MainWindow w;
#ifndef MOBILE
    //加载窗口位置  
    QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(),
                   QSettings::IniFormat);
    QScreen *pScreen = QGuiApplication::primaryScreen();
    int top = conf.value("UI/MainWindow/top",
           (pScreen->availableGeometry().height() - w.height()) >> 1).toInt();
    int left = conf.value("UI/MainWindow/left",
           (pScreen->availableGeometry().width() - w.width()) >> 1).toInt();
    int Width = conf.value("UI/MainWindow/width", w.geometry().width()).toInt();
    int Height = conf.value("UI/MainWindow/height",
                            w.geometry().height()).toInt();
    w.resize(Width, Height);
    w.move(left, top);
#endif
    w.show();

#ifdef RABBITIM_USE_LIBCURL
    //检查版本更新  
    CDownLoadHandleVersionFile update;
    update.Start();
#endif
    //*/

    /*以下为视频捕获、显示测试代码(CFrmPlayer::TestCamera())  
#ifdef DEBUG
    CFrmPlayer player;
    player.TestCamera();
    player.show();
#endif
    //*/
    return app.exec();
}
