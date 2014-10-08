#include "Manager.h"
#include "ManageMessageDialogBigScreen.h"
#include "FileTransfer/ManageFileTransfer.h"

CManager::CManager()
{
}

CManager::~CManager()
{
}

int CManager::Init(const QString &szId)
{
    //注意:初始化顺序  
    GetManageUser()->Init(szId);
    GetManageMessageDialog()->Init(szId);
    GetRecentMessage()->Init(szId);
    return 0;
}

int CManager::Clean()
{
    //注意:清理顺序  
    GetRecentMessage()->Clean();
    GetManageMessageDialog()->Clean();
    GetManageUser()->Clean();
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

QSharedPointer<CManageFileTransfer> CManager::GetFileTransfer()
{
    static QSharedPointer<CManageFileTransfer> file(new CManageFileTransfer);
    return file;
}
