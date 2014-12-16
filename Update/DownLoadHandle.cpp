#include "DownLoadHandle.h"
#include "Global/Global.h"
#include "Version.h"
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

    return 0;
}

CDownLoadHandleVersionFile::CDownLoadHandleVersionFile(const std::string &szFile, CDlgUpdate *parent)
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
    emit m_pDlg->sigDownLoadVersionFile(nErrorCode);
    return 0;
}
