#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name();

    //本地化QT资源
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);
    //本地化程序资源
    QTranslator myappTranslator;
    myappTranslator.load("app_" + QLocale::system().name(), a.applicationDirPath());
    a.installTranslator(&myappTranslator);

    MainWindow w;
    w.show();
    return a.exec();
}
