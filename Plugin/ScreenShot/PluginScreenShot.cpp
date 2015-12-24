#include "PluginScreenShot.h"
#include "DlgScreenShot.h"
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QStandardPaths>
#include "Tool.h"
#include "FileTransfer/ManageFileTransfer.h"
#include "QRCode.h"

CPluginScreenShot::CPluginScreenShot(QObject *parent) :
    QObject(parent), 
    CPluginApp()
{
}

int CPluginScreenShot::Init(const QString &szId)
{
    m_szLocaleId = szId;
    return 0;
}

int CPluginScreenShot::Clean()
{
    return 0;
}

int CPluginScreenShot::Open(void *pPara, QWidget *parent)
{
    CDlgScreenShot dlg(parent);
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
        else if(CGlobal::E_TO_QRCODE == type)
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
            QString szFilter =  tr("Images (*.PNG *.XPM *.JPG *.BMP, *.PPM, *.TIFF, *.XBM)");
            QString szDir = CGlobalDir::Instance()->GetDirReceiveFile()
                    + QDir::separator() + "grabbedImage_"
                    + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".PNG";
            szFile = CTool::FileDialog(&dlg, szDir, szFilter, tr("Save"), QFileDialog::AcceptSave);
            if(!szFile.isEmpty())
            {
                QFile f(szFile);
                if(f.exists())
                {
                    if(QMessageBox::No ==
                            QMessageBox::warning(&dlg, tr("Save"), 
                                                 tr("File is exists. Do you save it?"),
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
        else
        {
            QString fileName = "ShotScreen" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss.PNG");
            QString filePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) +QDir::separator() +  fileName;
            LOG_MODEL_DEBUG("Message", QString("filePath = %1").arg(filePath).toLocal8Bit().data());
            bool isOk = image.save(filePath);
            if(isOk)
            {
                CUser* user = (CUser*)pPara;
                if(user)
                    GETMANAGER->GetFileTransfer()->SendFile(user->GetInfo()->GetId(), filePath);
            }
            else
            {
                LOG_MODEL_ERROR("Message", "save file [%s] is error", filePath.toStdString().c_str());
            }
        }
    }
    return 0;
}

int CPluginScreenShot::Close()
{
    return 0;
}

QString CPluginScreenShot::ID()
{
    return "ScreenShot";
}

QString CPluginScreenShot::Name()
{
    return tr("ScreenShot");
}

QSet<QString> CPluginScreenShot::Group()
{
    return QSet<QString>() << tr("Tools");
}

QIcon CPluginScreenShot::Icon()
{
    return QIcon(":/icon/Cut");
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ScreenShot, CPluginScreenShot)
#endif // QT_VERSION < 0x050000
