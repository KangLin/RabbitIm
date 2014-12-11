#include "DownLoadHandle.h"
#include "Global/Global.h"
#include <QtXml>
#include "Version.h"
#include <QMessageBox>
#include "MainWindow.h"
#include <QFile>

CDownLoadHandleFile::CDownLoadHandleFile(QObject *parent):QObject(parent)
{}

CDownLoadHandleFile::~CDownLoadHandleFile()
{}

int CDownLoadHandleFile::SetFile(const QString &szFile)
{
    m_szFile = szFile;
    return 0;
}

int CDownLoadHandleFile::SetMd5sum(const QString &szMd5)
{
    m_szFileMd5sum = szMd5;
    return 0;
}

int CDownLoadHandleFile::OnEnd(int nErrorCode)
{
    if(nErrorCode)
    {
        //TODO:增加错误提示，向主窗口发送信号  
        LOG_MODEL_DEBUG("Update", "file download error:0x%X", nErrorCode);
        return nErrorCode;
    }

    //检查文件是否正确，计算md5校验和  
    QByteArray ba = CTool::GetFileMd5Sum(m_szFile);
    if(ba.toHex() != m_szFileMd5sum)
    {
        LOG_MODEL_ERROR("Update", "md5sum is error.");
        return -1;
    }

    //启动安装程序  
    m_Process.start(m_szFile);
    //关闭程序  
    MainWindow* pMain = (MainWindow*)GET_MAINWINDOW;
    pMain->close();
    return 0;
}

CDownLoadHandleVersionFile::CDownLoadHandleVersionFile(const std::string &szFile, QObject *parent): QObject(parent)
{
    m_szFile = szFile;
}

int CDownLoadHandleVersionFile::SetFile(const std::string &szFile)
{
    m_szFile = szFile;
    return 0;
}

int CDownLoadHandleVersionFile::OnEnd(int nErrorCode)
{
    if(m_szFile.empty())
        return -1;

    QFile file(m_szFile.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        LOG_MODEL_ERROR("Update", "Don't open file:%s", m_szFile.c_str());
        return -2;
    }

    QString szErr;
    QDomDocument doc;
    if(!doc.setContent(&file, &szErr))
    {
        LOG_MODEL_ERROR("Update", "doc.setContent error:%s", szErr.toStdString().c_str());
        return -3;
    }

    if(doc.isNull())
    {
        LOG_MODEL_ERROR("Update", "doc is null");
        return -4;
    }

    QDomElement startElem = doc.documentElement();
    LOG_MODEL_DEBUG("Update", "root:%s", startElem.tagName().toStdString().c_str());
    QString szMajorVersion = startElem.firstChildElement("MAJOR_VERSION_NUMBER").text();
    QString szMinorVersion = startElem.firstChildElement("MINOR_VERSION_NUMBER").text();
    QString szRevisionVersion = startElem.firstChildElement("REVISION_VERSION_NUMBER").text();
    if(szMajorVersion.toInt() <= MAJOR_VERSION_NUMBER)
    {
        if(szMinorVersion.toInt() <= MINOR_VERSION_NUMBER)
        {
            if(szRevisionVersion.toInt() <= REVISION_VERSION_NUMBER)
            {
                LOG_MODEL_DEBUG("Update", "Is already the newest version.");
                return 0;
            }
        }
    }

    QString szUrl = startElem.firstChildElement("URL").text();
    QString szMd5sum = startElem.firstChildElement("MD5SUM").text();
    QUrl url(szUrl);
    QString szDownLoadFile = CGlobal::Instance()->GetDirApplicationDownLoad() + QDir::separator() + url.fileName();
    QString szForce = startElem.firstChildElement("FORCE").text();
    if(szForce == "true")//强制更新  
    {
        m_DownLoadFile.SetFile(szDownLoadFile);
        m_DownLoadFile.SetMd5sum(szMd5sum);
        return m_DownLoad.Start(szUrl.toStdString(), szDownLoadFile.toStdString(), &m_DownLoadFile);
    }

    QString szText;
    if(QMessageBox::Ok != 
       QMessageBox::information(NULL, tr("Update"), szText, QMessageBox::Ok, QMessageBox::No))
    {
        return 0;
    }

    m_DownLoadFile.SetFile(szDownLoadFile);
    m_DownLoadFile.SetMd5sum(szMd5sum);
    return m_DownLoad.Start(szUrl.toStdString(), szDownLoadFile.toStdString(), &m_DownLoadFile);
}
