#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name();
    //本地化QT资源
    QTranslator qtTranslator;
    qtTranslator.load(QString("qt_") + locale + ".qm", a.applicationDirPath());
    a.installTranslator(&qtTranslator);
    //本地化程序资源
    QTranslator translator;
    translator.load(QString("app_") + locale + ".qm", a.applicationDirPath());
    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    return a.exec();
}
