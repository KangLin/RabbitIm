#include "ManagePluginApp.h"
#include "Global/Global.h"


CManagePluginApp::CManagePluginApp(QObject *parent)
    : CManage(parent)
{
}

CManagePluginApp::~CManagePluginApp()
{
}

int CManagePluginApp::LoginInit(const QString &szId)
{
    int nRet = 0;

    LoadFromStorage(szId);
    std::map<QString, CPluginApp* >::iterator it;
    for(it = m_Plugins.begin(); it != m_Plugins.end(); it++)
    {
        CPluginApp* plugin = it->second;
        plugin->Init(szId);
    }
    return nRet;
}

int CManagePluginApp::LogoutClean()
{
    SaveToStorage();
    std::map<QString, CPluginApp* >::iterator it;
    for(it = m_Plugins.begin(); it != m_Plugins.end(); it++)
    {
        CPluginApp* plugin = it->second;
        plugin->Clean();
    }
    return 0;
}

int CManagePluginApp::RegisterPlugin(const QString &szId,
                                     CPluginApp* plugin)
{
    if(m_Plugins.find(szId) != m_Plugins.end())
    {
        LOG_MODEL_ERROR("CManagePlugin", "Plugin [%s] is registered",
                        szId.toStdString().c_str());
        return -1;
    }
    m_Plugins.insert(std::pair<QString, CPluginApp* >(szId, plugin));
    emit sigChangedAdd(szId);
    return 0;
}

int CManagePluginApp::UnregisterPlugin(const QString &szId)
{
    std::map<QString, CPluginApp* >::iterator it;
    it = m_Plugins.find(szId);
    if(m_Plugins.end() == it)
        return 0;

    m_Plugins.erase(szId);
    emit sigChangedRemove(szId);
    return 0;
}

CPluginApp *CManagePluginApp::GetPlugin(const QString &szId)
{
    std::map<QString, CPluginApp* >::iterator it;
    it = m_Plugins.find(szId);
    if(m_Plugins.end() == it)
        return NULL;
    return it->second;
}

std::list<CPluginApp* > CManagePluginApp::GetAllPlugins()
{
    std::list<CPluginApp* > lstPlugins;
    std::map<QString, CPluginApp* >::iterator it;
    for(it = m_Plugins.begin(); it != m_Plugins.end(); it++)
        lstPlugins.push_back(it->second);
    return lstPlugins;
}

int CManagePluginApp::AddFavority(const QString &szId)
{
    bool bExist = false;
    std::list<QString>::iterator it;
    for(it = m_FavorityPlugins.begin(); it != m_FavorityPlugins.end(); it++)
    {
        if(*it == szId)
        {
            bExist = true;
            break;
        }
    }
    if(bExist)
        return -1;
    m_FavorityPlugins.push_back(szId);
    emit sigAddFavority(szId);
    return 0;
}

int CManagePluginApp::RemoveFavority(const QString &szId)
{
    std::list<QString>::iterator it;
    for(it = m_FavorityPlugins.begin(); it != m_FavorityPlugins.end(); it++)
    {
        if(*it == szId)
        {
            m_FavorityPlugins.erase(it);
            emit sigRemoveFavority(szId);
            break;
        }
    }
    return 0;
}

std::list<QString> CManagePluginApp::GetFavorites()
{
    return m_FavorityPlugins;
}

int CManagePluginApp::LoadFromStorage(const QString &szId)
{
    int nRet = 0;
    QString szFile = CGlobalDir::Instance()->GetDirUserData(szId) 
            + QDir::separator() + "PluginAppFavorites.dat";

    QFile in(szFile);
    if(!in.open(QFile::ReadOnly))
    {
        LOG_MODEL_WARNING("CManagePlugin", "Don't open file:%s", szFile.toStdString().c_str());
        return -1;
    }

    try{
        QDataStream s(&in);
        
        //版本号  
        int nVersion = 0;
        s >> nVersion;
        int nSize = 0;
        s >> nSize;
        QString szPlugin;
        while(nSize--)
        {
            s >> szPlugin;
            m_FavorityPlugins.push_back(szPlugin);
        }
    }
    catch(...)
    {
        LOG_MODEL_ERROR("CManagePlugin", "CManagePlugin::LoadFromStorage exception");
        nRet = -1;
    }

    in.close();
    return nRet;
}

int CManagePluginApp::SaveToStorage()
{
    int nRet = 0;
    if(USER_INFO_LOCALE.isNull())
    {
        LOG_MODEL_ERROR("CManagePlugin", "USER_INFO_LOCALE is null");
        return -1;
    }
    QString szFile = CGlobalDir::Instance()->GetDirUserData(USER_INFO_LOCALE->GetInfo()->GetId()) 
            + QDir::separator() + "PluginAppFavorites.dat";

    QFile out(szFile);
    if(!out.open(QFile::WriteOnly))
    {
        LOG_MODEL_WARNING("CManagePlugin", "Don't open file:%s", szFile.toStdString().c_str());
        return -1;
    }

    try
    {
        QDataStream s(&out);
        //版本号  
        int nVersion = 1;
        s << nVersion;
        int nSize = m_FavorityPlugins.size();
        s << nSize;
        std::list<QString>::iterator it;
        for(it = m_FavorityPlugins.begin(); it != m_FavorityPlugins.end(); it++)
        {
            QString szPlugin;
            szPlugin = *it;
            s << szPlugin;
        }
    }
    catch(...)
    {
        LOG_MODEL_ERROR("CManagePlugin", "CManagePlugin::SaveToStorage exception");
        return -1;
    }

    out.close();
    return nRet;
}
