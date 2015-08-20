#include "Manager.h"
#include "ManageMessageDialogBigScreen.h"
#include "FileTransfer/ManageFileTransfer.h"
#ifdef RABBITIM_USE_QXMPP
    #include "ManagerXmpp.h"
#endif
#include "Global/Global.h"

CManager::CManager()
{
}

CManager::~CManager()
{
    LOG_MODEL_DEBUG("CManager", "CManager::~CManager()");
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
#ifdef RABBITIM_USE_QXMPP
        case XMPP:
            pManager = (CManager*)new CManagerXmpp;
            break;
#endif
        default:
            LOG_MODEL_ERROR("CManager", "Don't support manager type:%d", type);
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
    LOG_MODEL_DEBUG("CManager", "CManager::LoginInit()");
    //注意:初始化顺序  
    GetManageUser()->Init(szId);
    GetManageMessageDialog()->Init(szId);
    GetManageGroupChat()->Init(szId);
    GetRecentMessage()->Init(szId);
    GetFileTransfer()->Init(szId);
    GetCall()->Init(szId);
    GetManagePlugins()->Init(szId);
    return 0;
}

int CManager::LogoutClean()
{
    LOG_MODEL_DEBUG("CManager", "CManager::LogoutClean()");
    //注意:清理顺序  
    GetManagePlugins()->Clean();
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

QSharedPointer<CManagePlugin> CManager::GetManagePlugins()
{
    static QSharedPointer<CManagePlugin> plugin(new CManagePlugin());
    return plugin;
}
