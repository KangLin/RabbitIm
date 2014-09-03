#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>
#include "Widgets/FrmVideo/FrmPlayer.h"
#include "Tool.h"
#include <QDebug>
#include "Global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置插件路径(msvc 下没有用）   
    a.addLibraryPath(a.applicationDirPath() + QDir::separator() + "plugins");
    a.addLibraryPath(a.applicationDirPath());
    a.addLibraryPath(CGlobal::Instance()->GetDirApplication());
    a.addLibraryPath(CGlobal::Instance()->GetDirApplication() + QDir::separator() + "plugins");

    CTool::SetFFmpegLog();

    /*
    QString szLocale = QLocale::system().name();
    LOG_MODEL_DEBUG("main", "locale language:%s", szLocale.toStdString().c_str());

    //本地化QT资源  
    QTranslator qtTranslator;
#ifdef DEBUG
    qtTranslator.load("qt_" + szLocale,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    LOG_MODEL_DEBUG("main", 
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath).toStdString().c_str());
#else
    qtTranslator.load("qt_" + szLocale,
                      QCoreApplication::applicationDirPath());
#endif
    qApp->installTranslator(&qtTranslator);
    //本地化程序资源  
    QTranslator myappTranslator;
    //把翻译文件放在了应用程序目录下,这样可以结约内存,适用于很多语言版本  
    //myappTranslator.load("app_" + locale, a.applicationDirPath());
    //把翻译文件放在了程序资源中  
    myappTranslator.load(":/translations/" + szLocale);
    qApp->installTranslator(&myappTranslator);//*/

    //*从配置文件中加载应用程序样式  
    QSettings conf(CGlobal::Instance()->GetApplicationConfigureFile(), QSettings::IniFormat);
    QString szFile(conf.value("UI/StyleSheet", ":/sink/Blue").toString());
    QFile file(szFile);//从资源文件中加载  
    if(file.open(QFile::ReadOnly))
    {
        QString stylesheet= file.readAll();
        qApp->setStyleSheet(stylesheet);
        file.close();
    }
    else
    {
        LOG_MODEL_ERROR("app", "file open file [%s] fail:%d",
                        szFile.toStdString().c_str(), file.error());
    }//*/

    //*
    QDesktopWidget *pDesk = QApplication::desktop();    
    MainWindow w;
    w.move((pDesk->width() - w.width()) / 2, (pDesk->height() - w.height()) / 2);
    w.show();//*/

    /*以下为视频捕获、显示测试代码(CFrmPlayer::TestCamera())  
    CFrmPlayer player;
    player.show();//*/

    return a.exec();
}
