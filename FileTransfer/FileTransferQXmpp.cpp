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

QString CFileTransferQXmpp::GetFile()
{
    return m_pJob->fileName();
}
