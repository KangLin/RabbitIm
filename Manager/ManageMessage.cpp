#include "ManageMessage.h"

CManageMessage::CManageMessage(QObject *parent) :
    QObject(parent)
{
}

int CManageMessage::Init(const QString &szId)
{
    int nRet = 0;
    return nRet;
}

int CManageMessage::Clean()
{
    int nRet = 0;
    return nRet;
}

CMessage* CManageMessage::GetMessage(const QString &szId)
{
    CMessage* pMessage = NULL;
    
    return pMessage;
}
