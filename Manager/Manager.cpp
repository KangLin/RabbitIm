#include "Manager.h"
#include "ManageMessageDialogBigScreen.h"

CManager::CManager()    
{
}

CManager::~CManager()
{
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

int CManager::Init(const QString &szId)
{
    GetGlobalUser()->Init(szId);
    GetManageMessageDialog()->Init(szId);
    GetManageMessage()->Init(szId);
    return 0;
}

int CManager::Clean()
{
        GetGlobalUser()->Clean();
        GetManageMessageDialog()->Clean();
        GetManageMessage()->Clean();
        return 0;
}

CManageMessage* CManager::GetManageMessage()
{
    CManageMessage message;
    return &message;
}
