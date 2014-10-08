#include "ManageFileTransfer.h"
//#include "FileTransferQXmpp.h"

CManageFileTransfer::CManageFileTransfer(QObject *parent) :
    QObject(parent)
{
}

CManageFileTransfer::~CManageFileTransfer()
{
}

int CManageFileTransfer::SendFile(const QString &szId, const QString &szFile, const QString &szDescription)
{
    int nRet = 0;
    //QSharedPointer<CFileTransfer> file(new CFileTransferQXmpp);
    
    return nRet;
}
