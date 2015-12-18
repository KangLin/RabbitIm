#include "PluginAppManage.h"
#include "Global/Global.h"
#include <QPluginLoader>
#include <QApplication>

CPluginAppManage::CPluginAppManage(QObject *parent)
    : CManage(parent)
{
    foreach (QObject *plugin, QPluginLoader::staticInstances())
    {
        QSharedPointer<CPluginApp> pluginApp(qobject_cast<CPluginApp *>(plugin));
        if(!pluginApp.isNull())
        {
            pluginApp->InitInstance();
            this->RegisterPlugin(pluginApp->ID(), pluginApp);
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
}

int CPluginAppManage::FindPlugins(QDir dir)
{
    QString fileName;
    foreach (fileName, dir.entryList(QDir::Files)) {
        QString szPlugins = dir.absoluteFilePath(fileName);
        QPluginLoader loader(szPlugins);
        QObject *plugin = loader.instance();
        if (plugin) {
            QSharedPointer<CPluginApp> pluginApp(qobject_cast<CPluginApp *>(plugin));
            if(!pluginApp.isNull())
            {
                pluginApp->InitInstance(dir.absolutePath());
                this->RegisterPlugin(pluginApp->ID(), pluginApp);
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

CPluginAppManage::~CPluginAppManage()
{
}

int CPluginAppManage::Init(const QString &szId)
{
    int nRet = 0;

    LoadFromStorage(szId);
    std::map<QString, QSharedPointer<CPluginApp> >::iterator it;
    for(it = m_Plugins.begin(); it != m_Plugins.end(); it++)
    {
        QSharedPointer<CPluginApp> plugin = it->second;
        plugin->Init(szId);
    }
    return nRet;
}

int CPluginAppManage::Clean()
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

int CPluginAppManage::RegisterPlugin(const QString &szId,
                                  QSharedPointer<CPluginApp> plugin)
{
    if(m_Plugins.find(szId) != m_Plugins.end())
    {
        LOG_MODEL_ERROR("CManagePlugin", "Plugin [%s] is registered",
                        szId.toStdString().c_str());
        return -1;
    }
    m_Plugins.insert(std::pair<QString, QSharedPointer<CPluginApp> >(szId, plugin));
    emit sigChangedAdd(szId);
    return 0;
}

int CPluginAppManage::UnregisterPlugin(const QString &szId)
{
    std::map<QString, QSharedPointer<CPluginApp> >::iterator it;
    it = m_Plugins.find(szId);
    if(m_Plugins.end() == it)
        return 0;

    m_Plugins.erase(szId);
    emit sigChangedRemove(szId);
    return 0;
}

QSharedPointer<CPluginApp> CPluginAppManage::GetPlugin(const QString &szId)
{
    std::map<QString, QSharedPointer<CPluginApp> >::iterator it;
    it = m_Plugins.find(szId);
    if(m_Plugins.end() == it)
        return QSharedPointer<CPluginApp>();
    return it->second;
}

std::list<QSharedPointer<CPluginApp> > CPluginAppManage::GetAllPlugins()
{
    std::list<QSharedPointer<CPluginApp> > lstPlugins;
    std::map<QString, QSharedPointer<CPluginApp> >::iterator it;
    for(it = m_Plugins.begin(); it != m_Plugins.end(); it++)
        lstPlugins.push_back(it->second);
    return lstPlugins;
}

int CPluginAppManage::AddFavority(const QString &szId)
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

int CPluginAppManage::RemoveFavority(const QString &szId)
{
    std::list<QString>::iterator it;
    for(it = m_FavorityPlugins.begin(); it != m_FavorityPlugins.end(); it++)
    {
        if(*it == szId)
        {
            m_FavorityPlugins.erase(it);
            //emit sigRemoveFavority(plugin);
            break;
        }
    }
    return 0;
}

std::list<QString> CPluginAppManage::GetFavorites()
{
    return m_FavorityPlugins;
}

int CPluginAppManage::LoadFromStorage(const QString &szId)
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

int CPluginAppManage::SaveToStorage()
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
