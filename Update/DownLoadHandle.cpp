#include "DownLoadHandle.h"
#include "Version.h"
#include "DlgUpdate.h"
#include "Global/Global.h"
#include "MainWindow.h"
#include <QDir>
#include <QtXml>

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

    return 0;
}

CDownLoadHandleVersionFile::CDownLoadHandleVersionFile()
{
}

int CDownLoadHandleVersionFile::OnError(int nErrorCode, const std::string &szErr)
{
    Q_UNUSED(szErr);
    LOG_MODEL_ERROR("CDownLoadHandleVersionFile", "Download version file error:%d", nErrorCode);
    return 0;
    //emit GET_MAINWINDOW->sigUpdateExec(nErrorCode, m_szFile);
    //return 0;
}

int CDownLoadHandleVersionFile::OnEnd(int nErrorCode)
{
    if(m_szFile.isEmpty())
        return -1;

    if(nErrorCode)
    {
        LOG_MODEL_ERROR("Update", "Don't open version configure file:%s", m_szFile.toStdString().c_str());
        return -2;
    }

    QFile file(m_szFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        LOG_MODEL_ERROR("Update", "Don't open version configure file:%s", m_szFile.toStdString().c_str());
        return -3;
    }

    QString szErr;
    QDomDocument doc;
    if(!doc.setContent(&file, &szErr))
    {
        LOG_MODEL_ERROR("Update", "doc.setContent error:%s", szErr.toStdString().c_str());
        return -4;
    }

    file.close();

    if(doc.isNull())
    {
        LOG_MODEL_ERROR("Update", "doc is null");
        return -5;
    }

    CGlobal::Instance()->SetUpdateDate(QDateTime::currentDateTime());

    QDomElement startElem = doc.documentElement();
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
#ifdef RABBITIM_USE_LIBCURL
    emit GET_MAINWINDOW->sigUpdateExec(nErrorCode, m_szFile);
#endif
    return 0;
}

int CDownLoadHandleVersionFile::Start()
{
    QDateTime d = CGlobal::Instance()->GetUpdateDate();
    int nDays = d.daysTo(QDateTime::currentDateTime());
    CGlobal::E_UPDATE updateType = CGlobal::Instance()->GetUpdate();
    switch(updateType)
    {
    case CGlobal::E_UPDATE_EVERY_TIME:
        break;
    case CGlobal::E_UPDATE_DAY:
        if(nDays < 1)
            return 0;
        break;
    case CGlobal::E_UPDATE_WEEK:
        if(nDays < 7)
            return 0;
        break;
    case CGlobal::E_UPDATE_MONTH:
        if(nDays < 30)
            return 0;
        break;
    default:
        return 0;
    }

    QString szFile = "Update_";
    szFile += RABBITIM_SYSTEM;
    szFile += ".xml";
    m_szFile = CGlobalDir::Instance()->GetDirApplicationDownLoad()
            + QDir::separator() + szFile;
    //TODO:设置下载版本文件  
    QString szUrl = "https://github.com/KangLin/rabbitim/blob/master/Update/" + szFile;

    m_DownLoad.Start(szUrl.toStdString(),
                     m_szFile.toStdString(),
                     this, 
                     1);

    return 0;
}
