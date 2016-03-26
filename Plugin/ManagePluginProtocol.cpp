#include "ManagePluginProtocol.h"
#include "Global/Global.h"
#include <QPluginLoader>
#include <QApplication>

CManagePluginProtocol::CManagePluginProtocol(QObject *parent)
 : CManage(parent)
{
}

int CManagePluginProtocol::LoginInit(const QString &szId)
{
    return 0;
}

int CManagePluginProtocol::LogoutClean()
{
    return 0;
}

int CManagePluginProtocol::RegisterPlugin(const QString &szProtocol,
                             CPluginProtocol *plugin)
{
    if(m_Plugins.find(szProtocol) != m_Plugins.end())
    {
        LOG_MODEL_ERROR("CManagePluginProtocol", "Plugin [%s] is registered",
                        szProtocol.toStdString().c_str());
        return -1;
    }
    m_Plugins.insert(std::pair<QString,
                     CPluginProtocol* >(szProtocol, plugin));
    return 0;
}

int CManagePluginProtocol::UnregisterPlugin(const QString &szProtocol)
{
    std::map<QString, CPluginProtocol* >::iterator it;
    it = m_Plugins.find(szProtocol);
    if(m_Plugins.end() == it)
        return 0;

    m_Plugins.erase(szProtocol);
    return 0;
}

CPluginProtocol *CManagePluginProtocol::GetPlugin(
        const QString &szProtocol)
{
    std::map<QString, CPluginProtocol* >::iterator it;
    it = m_Plugins.find(szProtocol);
    if(m_Plugins.end() == it)
    {
        LOG_MODEL_ERROR("CManagePluginProtocol", "GetPlugin %s is fail",
                        szProtocol.toStdString().c_str());
        return NULL;
    }
    return it->second;
}

std::list<CPluginProtocol*> CManagePluginProtocol::GetAllPlugins()
{
    std::list<CPluginProtocol* > lstPlugins;
    std::map<QString, CPluginProtocol* >::iterator it;
    for(it = m_Plugins.begin(); it != m_Plugins.end(); it++)
        lstPlugins.push_back(it->second);
    return lstPlugins;
}
