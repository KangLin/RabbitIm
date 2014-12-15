#include "DownLoadHandle.h"
#include "Global/Global.h"
#include <QtXml>
#include "Version.h"
#include <QMessageBox>
#include "MainWindow.h"
#include <QFile>
#include "DlgUpdate.h"

CDownLoadHandleFile::CDownLoadHandleFile(CDlgUpdate *parent):QObject(parent)
{
    m_pDlg = parent;
}

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

int CDownLoadHandleFile::OnProgress(double total, double now)
{
    emit m_pDlg->sigProcess(total, now);
    return 0;
}

int CDownLoadHandleFile::OnError(int nErrorCode, const std::string &szErr)
{
    emit m_pDlg->sigError(nErrorCode, szErr.c_str());
    return 0;
}

int CDownLoadHandleFile::OnEnd(int nErrorCode)
{
    emit m_pDlg->sigDownLoadEnd(nErrorCode);

    if(nErrorCode)
    {
        LOG_MODEL_DEBUG("Update", "file download error:0x%X", nErrorCode);
        QString szErr = tr("Download file fail:%1").arg(m_szFile);
        emit m_pDlg->sigError(nErrorCode, szErr);
        return nErrorCode;
    }

    //检查文件是否正确，计算md5校验和  
    if(CTool::GetFileMd5SumString(m_szFile) != m_szFileMd5sum)
    {
        LOG_MODEL_ERROR("Update", "md5sum is error.");
        QString szErr = tr("Download file md5sum error:%1").arg(m_szFile);
        emit m_pDlg->sigError(-1, szErr);
        return -1;
    }

    //修改文件执行权限
    QFileInfo info(m_szFile);
    if(!info.permission(QFile::ExeUser))
    {
        //修改文件执行权限
        QString szErr = tr("Download file don't execute permissions. Please modify permission then manually  execute it.\n%1").arg(m_szFile);
        emit m_pDlg->sigError(-2, szErr);
        return -2;
    }

    //启动安装程序  
    if(!m_Process.startDetached(m_szFile))
     {
        LOG_MODEL_ERROR("Update", "open proess error:%s", m_szFile.toStdString().c_str());
        QString szErr = tr("Execute install program error.%1").arg(m_szFile);
        emit m_pDlg->sigError(-2, szErr);
        return -3;
    }
    //关闭程序  
    MainWindow* pMain = (MainWindow*)GET_MAINWINDOW;
    pMain->close();
    return 0;
}

CDownLoadHandleVersionFile::CDownLoadHandleVersionFile(const std::string &szFile, CDlgUpdate *parent): QObject(parent)
{
    m_szFile = szFile;
    m_pDlg = parent;
}

int CDownLoadHandleVersionFile::SetFile(const std::string &szFile)
{
    m_szFile = szFile;
    return 0;
}

int CDownLoadHandleVersionFile::OnError(int nErrorCode, const std::string &szErr)
{
    emit m_pDlg->sigError(nErrorCode, szErr.c_str());
    return 0;
}

int CDownLoadHandleVersionFile::OnEnd(int nErrorCode)
{
    if(m_szFile.empty())
        return -1;

    if(nErrorCode)
    {
        QString szErr = tr("Download file fail:%1").arg(m_szFile.c_str());
        emit m_pDlg->sigError(nErrorCode, szErr);
        return nErrorCode;
    }

    QFile file(m_szFile.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        LOG_MODEL_ERROR("Update", "Don't open file:%s", m_szFile.c_str());
        QString szErr = tr("Don't open file:%1").arg(m_szFile.c_str());
        emit m_pDlg->sigError(-2, szErr);
        return -2;
    }

    QString szErr;
    QDomDocument doc;
    if(!doc.setContent(&file, &szErr))
    {
        LOG_MODEL_ERROR("Update", "doc.setContent error:%s", szErr.toStdString().c_str());
        QString szErr = tr("File format error.%1").arg(m_szFile.c_str());
        emit m_pDlg->sigError(-3, szErr);
        return -3;
    }

    if(doc.isNull())
    {
        LOG_MODEL_ERROR("Update", "doc is null");
        QString szErr = tr("File format error.%1").arg(m_szFile.c_str());
        emit m_pDlg->sigError(-4, szErr);
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
                m_pDlg->accept();
                return 0;
            }
        }
    }

    m_szInfo = tr("New version:%1.%2.%3\n").arg(szMajorVersion, szMinorVersion, szRevisionVersion);
    m_szInfo += startElem.firstChildElement("INFO").text();
    m_szUrl = startElem.firstChildElement("URL").text();
    m_szMd5sum = startElem.firstChildElement("MD5SUM").text();
    QUrl url(m_szUrl);
    m_szDownLoadFile= CGlobal::Instance()->GetDirApplicationDownLoad() + QDir::separator() + url.fileName();
    QString szForce = startElem.firstChildElement("FORCE").text();
    if(szForce == "true")//强制更新  
    {
        emit m_pDlg->sigDownLoadStart(false);
        return 0;
    }

    emit m_pDlg->sigDownLoadStart(true);
    return 0;
}
