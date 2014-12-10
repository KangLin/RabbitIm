#include "MainWindow.h"
#include "Widgets/FrmVideo/FrmPlayer.h"
#include "Global/Global.h"
#include "Tool.h"
#include <QDebug>
#include <QApplication>
#include <QTranslator>

#ifdef RABBITIM_USER_LIBCURL
#include "Update/Update.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("RabbitIm");
    a.setOrganizationName("KangLin studio");

    //QFontDatabase::addApplicationFont("://DejaVuSans.ttf");
    //a.setFont(QFont(DejaVuSans));
    LOG_MODEL_DEBUG("main", "font:%s", a.font().family().toStdString().c_str());

    //设置插件路径(msvc 下没有用）   
    a.addLibraryPath(CGlobal::Instance()->GetDirApplication());
    a.addLibraryPath(CGlobal::Instance()->GetDirApplication() + QDir::separator() + "plugins");

    CTool::SetFFmpegLog();

    //*
    MainWindow w;
#ifndef MOBILE
    CTool::SetWindowsGeometry(&w);
#endif
    w.show();//*/

    /*以下为视频捕获、显示测试代码(CFrmPlayer::TestCamera())  
    CFrmPlayer player;
    player.TestCamera();
    player.show();//*/

    return a.exec();
}
