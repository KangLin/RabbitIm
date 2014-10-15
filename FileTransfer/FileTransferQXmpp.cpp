#include "FileTransferQXmpp.h"
#include "Global/Global.h"

CFileTransferQXmpp::CFileTransferQXmpp(QXmppTransferJob *pJob, QObject *parent) :
    CFileTransfer(parent)
{
    m_pJob = pJob;
}

int CFileTransferQXmpp::Accept(const QString &szFile)
{
    int nRet = 0;
    
    return nRet;
}

int CFileTransferQXmpp::Abort()
{
    return 0;
}

QUrl CFileTransferQXmpp::GetLocalFileUrl()
{
    return m_pJob->localFileUrl();
}

QString CFileTransferQXmpp::GetFile()
{
    return m_pJob->fileName();
}

qint64 CFileTransferQXmpp::GetFileSize()
{
    return m_pJob->fileSize();
}

CFileTransfer::Direction CFileTransferQXmpp::GetDirection()
{
    return (Direction) m_pJob->direction();
}

CFileTransfer::State CFileTransferQXmpp::GetState()
{
    return (State)m_pJob->state();
}

CFileTransfer::Error CFileTransferQXmpp::GetError()
{
    return (Error) m_pJob->error();
}
