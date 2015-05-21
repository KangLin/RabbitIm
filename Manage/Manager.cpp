#include "Manager.h"
#include "ManageMessageDialogBigScreen.h"
#include "FileTransfer/ManageFileTransfer.h"
#include "ManagerXmpp.h"

CManager::CManager()
{
}

CManager::~CManager()
{
}

CManager *CManager::Instance(MANAGER_TYPE type, bool bReset)
{
    static CManager* pManager = NULL;
    if(!pManager && bReset)
    {
        delete pManager;
    }

    if(!pManager)
    {
        switch(type)
        {
        case XMPP:
            pManager = (CManager*)new CManagerXmpp;
        default:
            ;
        }
    }
    return pManager;
}

int CManager::Init()
{
    return 0;
}

int CManager::Clean()
{
    return 0;
}

int CManager::LoginInit(const QString &szId)
{
    //注意:初始化顺序  
    GetManageUser()->Init(szId);
    GetManageMessageDialog()->Init(szId);
    GetManageGroupChat()->Init(szId);
    GetRecentMessage()->Init(szId);
    GetFileTransfer()->Init(szId);
    GetCall()->Init(szId);
    return 0;
}

int CManager::LogoutClean()
{
    //注意:清理顺序  
    GetCall()->Clean();
    GetFileTransfer()->Clean();
    GetRecentMessage()->Clean();
    GetManageGroupChat()->Clean();
    GetManageMessageDialog()->Clean();
    GetManageUser()->Clean();
    return 0;
}

QSharedPointer<CManageMessageDialog> CManager::GetManageMessageDialog()
{
    //TODO:根据操作系统来决定屏幕  
#ifdef MOBILE
    static QSharedPointer<CManageMessageDialog> manageMessageDialog(new CManageMessageDialog;
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

QSharedPointer<CManageCall> CManager::GetCall()
{
    static QSharedPointer<CManageCall> call(new CManageCall);
    return call;
}
