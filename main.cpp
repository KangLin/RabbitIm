#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("app_") + locale + ".qm");
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
