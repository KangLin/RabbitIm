#include "MainWindow.h"
#include "Widgets/FrmVideo/FrmPlayer.h"
#include "Global/Global.h"
#include "Tool.h"
#include <QDebug>
#include <QApplication>
#include <QTranslator>

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
    QDesktopWidget *pDesk = QApplication::desktop();    
    MainWindow w;
#ifdef MOBILE
    QScreen* pScreen = QApplication::primaryScreen();
    LOG_MODEL_DEBUG("main", "screen:size(%d,%d);availableSize(%d,%d);geometry(%d,%d);availableGeometry(%d,%d)",
                    pScreen->size().width(),
                    pScreen->size().height(),
                    pScreen->availableSize().width(),
                    pScreen->availableSize().height(),
                    pScreen->geometry().width(),
                    pScreen->geometry().height(),
                    pScreen->availableGeometry().width(),
                    pScreen->availableGeometry().height()
                    );
    LOG_MODEL_DEBUG("main", "desk:size(%d,%d);geometry(%d,%d);availableGeometry(%d,%d)",
                    pDesk->size().width(),
                    pDesk->size().height(),
                    pDesk->geometry().width(),
                    pDesk->geometry().height(),
                    pDesk->availableGeometry().width(),
                    pDesk->availableGeometry().height()
                    );
    w.setGeometry(pDesk->geometry());
#else
    w.move((pDesk->width() - w.width()) / 2, (pDesk->height() - w.height()) / 2);
#endif
    w.show();//*/

    /*以下为视频捕获、显示测试代码(CFrmPlayer::TestCamera())  
    CFrmPlayer player;
    player.TestCamera();
    player.show();//*/

    return a.exec();
}
