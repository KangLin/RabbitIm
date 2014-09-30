#include "Manager.h"
#include "ManageMessageDialogBigScreen.h"

CManager::CManager()    
{
}

CManager::~CManager()
{
}

int CManager::Init(const QString &szId)
{
    GetManageUser()->Init(szId);
    GetManageMessageDialog()->Init(szId);
    GetRecentMessage()->Init(szId);
    return 0;
}

int CManager::Clean()
{
    GetManageUser()->Clean();
    GetManageMessageDialog()->Clean();
    GetRecentMessage()->Clean();
    return 0;
}

QSharedPointer<CManageMessageDialog> CManager::GetManageMessageDialog()
{
#ifdef MOBILE
    static QSharedPointer<CManageMessageDialog> manageMessageDialog(new CManageMessageDialog);
#else
    static QSharedPointer<CManageMessageDialog> manageMessageDialog(new CManageMessageDialogBigScreen);
#endif
    return manageMessageDialog;
}

QSharedPointer<CManageRecentMessage> CManager::GetRecentMessage()
{
    static QSharedPointer<CManageRecentMessage> msg(new CManageRecentMessage);
    return msg;
}
