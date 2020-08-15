#include <QFile>
#include <QApplication>

#include "FrmFaceRecognizer.h"
#include "FrmFacePicture.h"

#include <QMetaType>
Q_DECLARE_METATYPE(cv::Mat)

int main(int argc, char *argv[])
{
    qRegisterMetaType<cv::Mat>("Mat");
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

    //CFrmFaceRecognizer w;
    CFrmFacePicture w;
    w.show();

    return a.exec();
}
