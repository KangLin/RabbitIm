#include "Manager.h"
#include "ManageMessageDialogBigScreen.h"
#include "FileTransfer/ManageFileTransfer.h"
#include "Global/Global.h"
#include <QPluginLoader>
#include <QApplication>

CManager::CManager():
    m_PluginProtocol(NULL),
    m_ManagePluginProtocol(new CManagePluginProtocol()),
    m_ManagePluginApp(new CManagePluginApp()),
    m_ManageFile(new CManageFileTransfer),
    m_ManageRecentMessage(new CManageRecentMessage),
    n_ManageMessageDialog(NULL)
{
    foreach (QObject *plugin, QPluginLoader::staticInstances())
    {
        CPluginApp* pluginApp = qobject_cast<CPluginApp *>(plugin);
        if(pluginApp)
        {
            pluginApp->InitInstance();
            GetManagePluginApp()->RegisterPlugin(pluginApp->ID(), pluginApp);
            continue;
        }
        CPluginProtocol* pluginProtocol = qobject_cast<CPluginProtocol* >(plugin);
        if(pluginProtocol)
        {
            pluginProtocol->InitInstance();
            GetManagePluginProtocol()->RegisterPlugin(pluginProtocol->ID(), pluginProtocol);
            continue;
        }
    }

    QDir pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
#ifndef Q_OS_ANDROID
    if(pluginsDir.cd("plugins"))
#endif
        FindPlugins(pluginsDir);

    //TODO:修改协议  
    ChangeProtolcol("qxmpp");
}

CManager::~CManager()
{
    LOG_MODEL_DEBUG("CManager", "CManager::~CManager()");
    //增加移除插件  
    std::list<CPluginProtocol*> lstProtocol = GetManagePluginProtocol()->GetAllPlugins();
    std::list<CPluginProtocol*>::iterator it;
    for(it = lstProtocol.begin(); it != lstProtocol.end(); it++)
    {
        CPluginProtocol* p = *it;
        p->ClearInstance();
        GetManagePluginProtocol()->UnregisterPlugin(p->ID());
    }
    
    std::list<CPluginApp*> lstApp = GetManagePluginApp()->GetAllPlugins();
    std::list<CPluginApp*>::iterator itApp;
    for(itApp = lstApp.begin(); itApp != lstApp.end(); itApp++)
    {
        CPluginApp* p = *itApp;
        p->ClearInstance();
        GetManagePluginApp()->UnregisterPlugin(p->ID());
    }
}

int CManager::ChangeProtolcol(QString szProtocol)
{
    if(m_PluginProtocol)
    {
        if(m_PluginProtocol->ID() == szProtocol)
            return 0;
    }
    m_PluginProtocol = GetManagePluginProtocol()->GetPlugin(szProtocol);
    Q_ASSERT(m_PluginProtocol);
    return 0;
}

QSharedPointer<CClient> CManager::GetClient()
{
    return m_PluginProtocol->GetClient();
}

QSharedPointer<CManageUser> CManager::GetManageUser()
{
    return m_PluginProtocol->GetManageUser();
}

QSharedPointer<CManageCall> CManager::GetCall()
{
    return m_PluginProtocol->GetCall();
}

QSharedPointer<CManageGroupChat> CManager::GetManageGroupChat()
{
    return m_PluginProtocol->GetManageGroupChat();
}

QSharedPointer<CUserInfo> CManager::NewUserInfo()
{
    return m_PluginProtocol->NewUserInfo();
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
    GetManageUser()->LoginInit(szId);
    GetManagePluginProtocol()->LoginInit(szId);
    GetManageMessageDialog()->LoginInit(szId);
    GetManageGroupChat()->LoginInit(szId);
    GetRecentMessage()->LoginInit(szId);
    GetFileTransfer()->LoginInit(szId);
    GetCall()->LoginInit(szId);
    GetManagePluginApp()->LoginInit(szId);
    return 0;
}

int CManager::LogoutClean()
{
    LOG_MODEL_DEBUG("CManager", "CManager::LogoutClean()");
    //注意:清理顺序  
    GetManagePluginApp()->LogoutClean();
    GetCall()->LogoutClean();
    GetFileTransfer()->LogoutClean();
    GetRecentMessage()->LogoutClean();
    GetManageGroupChat()->LogoutClean();
    GetManageMessageDialog()->LogoutClean();
    GetManagePluginProtocol()->LogoutClean();
    GetManageUser()->LogoutClean();
    return 0;
}

QSharedPointer<CManageMessageDialog> CManager::GetManageMessageDialog()
{
    if(n_ManageMessageDialog)
        return n_ManageMessageDialog;
    //TODO:根据操作系统来决定屏幕  
#ifdef MOBILE
    n_ManageMessageDialog = QSharedPointer<CManageMessageDialog>(new CManageMessageDialog);
#else
    n_ManageMessageDialog = QSharedPointer<CManageMessageDialog>
            (new CManageMessageDialogBigScreen);
#endif
    return n_ManageMessageDialog;
}

QSharedPointer<CManageRecentMessage> CManager::GetRecentMessage()
{
    return m_ManageRecentMessage;
}

QSharedPointer<CManageFileTransfer> CManager::GetFileTransfer()
{
    return m_ManageFile;
}

QSharedPointer<CManagePluginApp> CManager::GetManagePluginApp()
{
    return m_ManagePluginApp;
}

QSharedPointer<CManagePluginProtocol> CManager::GetManagePluginProtocol()
{
    return m_ManagePluginProtocol;
}

int CManager::FindPlugins(QDir dir)
{
    QString fileName;
    QStringList filters;
    filters << "*.so" << "*.dll";
    foreach (fileName, dir.entryList(filters, QDir::Files)) {
        QString szPlugins = dir.absoluteFilePath(fileName);
        QPluginLoader loader(szPlugins);
        QObject *plugin = loader.instance();
        if (plugin) {
            CPluginApp* pluginApp = qobject_cast<CPluginApp* >(plugin);
            if(pluginApp)
            {
                pluginApp->InitInstance(dir.absolutePath());
                GetManagePluginApp()->RegisterPlugin(pluginApp->ID(), pluginApp);
                continue;
            }
            CPluginProtocol* pluginProtocol = qobject_cast<CPluginProtocol* >(plugin);
            if(pluginProtocol)
            {
                pluginProtocol->InitInstance(dir.absolutePath());
                GetManagePluginProtocol()->RegisterPlugin(pluginProtocol->ID(), pluginProtocol);
                continue;
            }
        }else{
            LOG_MODEL_ERROR("CManager", "load plugin error:%s",
                            loader.errorString().toStdString().c_str());
        }
    }

    foreach (fileName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir pluginDir = dir;
        if(pluginDir.cd(fileName))
            FindPlugins(pluginDir);
    }

    return 0;
}
