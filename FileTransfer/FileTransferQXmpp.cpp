#include "FileTransferQXmpp.h"
#include "Global/Global.h"

CFileTransferQXmpp::CFileTransferQXmpp(QObject *parent) :
    CFileTransfer(parent)
{
}

int CFileTransferQXmpp::SendFile(const QString &szId, const QString &szFile, const QString &szDescription)
{
    int nRet = 0;
    
    return nRet;
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
