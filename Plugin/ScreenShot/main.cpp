#include <QFile>
#include <QApplication>
#include "DlgScreenShot.h"
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QStandardPaths>
#include "Tool.h"
#include "FileTransfer/ManageFileTransfer.h"
#include "QRCode.h"

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

    CManager manager;
    CGlobal::Instance()->SetManager(&manager);

    CDlgScreenShot dlg;
    if(dlg.exec() ==  QDialog::Accepted)
    {
        QImage image = dlg.getSelectedImg().toImage();
        CGlobal::E_SCREEN_SHOT_TO_TYPE type = CGlobal::Instance()->GetScreenShotToType();
        if(type == CGlobal::E_TO_CLIPBOARD)
        {
            //TODO:剪切板操作会引起警告：QImage::pixel: coordinate (308,398) out of range  
            QClipboard* clipboard = QApplication::clipboard();
            clipboard->setImage(image);//参数是否合适TODO  
        }
        else if(CGlobal::E_TO_QRCODE)
        {
            QString szText;
            /*QString fileName = "ShotScreen" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss.PNG");
            QString filePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) +QDir::separator() +  fileName;
            LOG_MODEL_DEBUG("Message", QString("filePath = %1").arg(filePath).toLocal8Bit().data());
            bool isOk = image.save(filePath);
            if(isOk)
            {
                return CQRCode::ProcessQRFile(filePath, szText);
             }*/
            return CQRCode::ProcessQImage(image, szText);
        }
        else if(type == CGlobal::E_TO_SAVE)
        {
            QString szFile;
            QString szFilter =  QObject::tr("Images (*.PNG *.XPM *.JPG *.BMP, *.PPM, *.TIFF, *.XBM)");
            QString szDir = CGlobalDir::Instance()->GetDirReceiveFile()
                    + QDir::separator() + "grabbedImage_"
                    + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".PNG";
            szFile = CTool::FileDialog(&dlg, szDir, szFilter, QObject::tr("Save"), QFileDialog::AcceptSave);
            if(!szFile.isEmpty())
            {
                QFile f(szFile);
                if(f.exists())
                {
                    if(QMessageBox::No ==
                            QMessageBox::warning(&dlg, QObject::tr("Save"), 
                                                 QObject::tr("File is exists. Do you save it?"),
                                                 QMessageBox::Ok, 
                                                 QMessageBox::No))
                    {
                        return 0;
                    }
                }
                bool isOk = image.save(szFile);
                if(!isOk)
                {
                    LOG_MODEL_ERROR("Message", "save file [%s] is error", szFile.toStdString().c_str());
                }
            }
        }
    }

    return 0;//a.exec();
}
