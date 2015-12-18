#include "Manager.h"
#include "ManageMessageDialogBigScreen.h"
#include "FileTransfer/ManageFileTransfer.h"
#include "Global/Global.h"
#include <QPluginLoader>
#include <QApplication>

CManager::CManager()
{
    foreach (QObject *plugin, QPluginLoader::staticInstances())
    {
        QSharedPointer<CPluginApp> pluginApp(qobject_cast<CPluginApp *>(plugin));
        if(!pluginApp.isNull())
        {
            pluginApp->InitInstance();
            GetManagePluginApp()->RegisterPlugin(pluginApp->ID(), pluginApp);
            continue;
        }
        QSharedPointer<CPluginProtocol> pluginProtocol(qobject_cast<CPluginProtocol* >(plugin));
        if(!pluginProtocol.isNull())
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

    ChangeProtolcol("qxmpp");
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
        pManager = new CManager;
    }
    return pManager;
}

int CManager::ChangeProtolcol(QString szProtocol)
{
    if(!m_PluginProtocol.isNull())
    {
        if(m_PluginProtocol->ID() == szProtocol)
            return 0;
    }
    m_PluginProtocol = GetManagePluginProtocol()->GetPlugin(szProtocol);
    Q_ASSERT(!m_PluginProtocol.isNull());
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
    GetManageUser()->Init(szId);
    GetManageMessageDialog()->Init(szId);
    GetManageGroupChat()->Init(szId);
    GetRecentMessage()->Init(szId);
    GetFileTransfer()->Init(szId);
    GetCall()->Init(szId);
    GetManagePluginApp()->Init(szId);
    GetManagePluginProtocol()->Init(szId);
    return 0;
}

int CManager::LogoutClean()
{
    LOG_MODEL_DEBUG("CManager", "CManager::LogoutClean()");
    //注意:清理顺序  
    GetManagePluginProtocol()->Clean();
    GetManagePluginApp()->Clean();
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

QSharedPointer<CManagePluginApp> CManager::GetManagePluginApp()
{
    static QSharedPointer<CManagePluginApp> pluginApp(new CManagePluginApp());
    return pluginApp;
}

QSharedPointer<CManagePluginProtocol> CManager::GetManagePluginProtocol()
{
    static QSharedPointer<CManagePluginProtocol> pluginProtocol(new CManagePluginProtocol());
    return pluginProtocol;
}

int CManager::FindPlugins(QDir dir)
{
    QString fileName;
    foreach (fileName, dir.entryList(QDir::Files)) {
        QString szPlugins = dir.absoluteFilePath(fileName);
        QPluginLoader loader(szPlugins);
        QObject *plugin = loader.instance();
        if (plugin) {
            QSharedPointer<CPluginApp> pluginApp(qobject_cast<CPluginApp* >(plugin));
            if(!pluginApp.isNull())
            {
                pluginApp->InitInstance(dir.absolutePath());
                GetManagePluginApp()->RegisterPlugin(pluginApp->ID(), pluginApp);
                continue;
            }
            QSharedPointer<CPluginProtocol> pluginProtocol(qobject_cast<CPluginProtocol* >(plugin));
            if(!pluginProtocol.isNull())
            {
                pluginProtocol->InitInstance(dir.absolutePath());
                GetManagePluginProtocol()->RegisterPlugin(pluginProtocol->ID(), pluginProtocol);
                continue;
            }
        }
    }

    foreach (fileName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir pluginDir = dir;
        if(pluginDir.cd(fileName))
            FindPlugins(pluginDir);
    }

    return 0;
}
