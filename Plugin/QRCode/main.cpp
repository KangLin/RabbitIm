#include <QFile>
#include <QApplication>
#include "FrmScanQRcode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //*从配置文件中加载应用程序样式  
    QString szFile = ":/qdarkstyle/style.qss";
    if(szFile.isEmpty())
        qApp->setStyleSheet("");
    else
    {
        QFile file(szFile);//从资源文件中加载  
        if(file.open(QFile::ReadOnly))
        {
            QString stylesheet= file.readAll();
            qApp->setStyleSheet(stylesheet);
            file.close();
        }
    }

    CFrmScanQRcode w;
    w.show();

    return a.exec();
}
