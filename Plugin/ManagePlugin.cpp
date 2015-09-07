#include "ManagePlugin.h"
#include "Global/Global.h"
#include "Plugin/Lbs/PluginAppMotion.h"
#include <QDir>

CManagePlugin::CManagePlugin(QObject *parent)
    : CManage(parent)
{
}

int CManagePlugin::Init(const QString &szId)
{
    int nRet = 0;
   
    //TODO:
    QSharedPointer<CPluginApp> pluginMotion(new CPluginAppMotion());
    RegisterPlugin(pluginMotion->Name(), pluginMotion);
    
    LoadFromStorage(szId);
    std::map<QString, QSharedPointer<CPluginApp> >::iterator it;
    for(it = m_Plugins.begin(); it != m_Plugins.end(); it++)
    {
        QSharedPointer<CPluginApp> plugin = it->second;
        plugin->Init(szId);
    }
    return nRet;
}

int CManagePlugin::Clean()
{
    SaveToStorage();
    std::map<QString, QSharedPointer<CPluginApp> >::iterator it;
    for(it = m_Plugins.begin(); it != m_Plugins.end(); it++)
    {
        QSharedPointer<CPluginApp> plugin = it->second;
        plugin->Clean();
    }
    return 0;
}

int CManagePlugin::RegisterPlugin(const QString &szName,
                                  QSharedPointer<CPluginApp> plugin)
{
    if(m_Plugins.find(szName) != m_Plugins.end())
    {
        LOG_MODEL_ERROR("CManagePlugin", "Plugin [%s] is registered",
                        szName.toStdString().c_str());
        return -1;
    }
    m_Plugins.insert(std::pair<QString, QSharedPointer<CPluginApp> >(szName, plugin));
    emit sigChangedAdd(szName);
    return 0;
}

int CManagePlugin::UnregisterPlugin(const QString &szName)
{
    if(m_Plugins.find(szName) == m_Plugins.end())
        return 0;
    m_Plugins.erase(szName);
    emit sigChangedRemove(szName);
    return 0;
}

QSharedPointer<CPluginApp> CManagePlugin::GetPlugin(const QString &szName)
{
    std::map<QString, QSharedPointer<CPluginApp> >::iterator it;
    it = m_Plugins.find(szName);
    if(m_Plugins.end() == it)
        return QSharedPointer<CPluginApp>();
    return it->second;
}

std::list<QSharedPointer<CPluginApp> > CManagePlugin::GetAllPlugins()
{
    std::list<QSharedPointer<CPluginApp> > lstPlugins;
    std::map<QString, QSharedPointer<CPluginApp> >::iterator it;
    for(it = m_Plugins.begin(); it != m_Plugins.end(); it++)
        lstPlugins.push_back(it->second);
    return lstPlugins;
}

int CManagePlugin::AddFavority(const QString &plugin)
{
    bool bExist = false;
    std::list<QString>::iterator it;
    for(it = m_FavorityPlugins.begin(); it != m_FavorityPlugins.end(); it++)
    {
        if(*it == plugin)
        {
            bExist = true;
            break;
        }
    }
    if(bExist)
        return -1;
    m_FavorityPlugins.push_back(plugin);
    emit sigAddFavority(plugin);
    return 0;
}

int CManagePlugin::RemoveFavority(const QString &plugin)
{
    std::list<QString>::iterator it;
    for(it = m_FavorityPlugins.begin(); it != m_FavorityPlugins.end(); it++)
    {
        if(*it == plugin)
        {
            m_FavorityPlugins.erase(it);
            //emit sigRemoveFavority(plugin);
            break;
        }
    }
    return 0;
}

std::list<QString> CManagePlugin::GetFavorites()
{
    return m_FavorityPlugins;
}

int CManagePlugin::LoadFromStorage(const QString &szId)
{
    int nRet = 0;
    QString szFile = CGlobalDir::Instance()->GetDirUserData(szId) 
            + QDir::separator() + "PluginFavorites.dat";

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

int CManagePlugin::SaveToStorage()
{
    int nRet = 0;
    if(USER_INFO_LOCALE.isNull())
    {
        LOG_MODEL_ERROR("CManagePlugin", "USER_INFO_LOCALE is null");
        return -1;
    }
    QString szFile = CGlobalDir::Instance()->GetDirUserData(USER_INFO_LOCALE->GetInfo()->GetId()) 
            + QDir::separator() + "PluginFavorites.dat";

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
