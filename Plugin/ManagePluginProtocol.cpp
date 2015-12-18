#include "ManagePluginProtocol.h"
#include "Global/Global.h"
#include <QPluginLoader>
#include <QApplication>

CManagePluginProtocol::CManagePluginProtocol(QObject *parent)
 : CManage(parent)
{
}

int CManagePluginProtocol::Init(const QString &szId)
{
    return 0;
}

int CManagePluginProtocol::Clean()
{
    return 0;
}

int CManagePluginProtocol::RegisterPlugin(const QString &szProtocol,
                             QSharedPointer<CPluginProtocol> plugin)
{
    if(m_Plugins.find(szProtocol) != m_Plugins.end())
    {
        LOG_MODEL_ERROR("CManagePluginProtocol", "Plugin [%s] is registered",
                        szProtocol.toStdString().c_str());
        return -1;
    }
    m_Plugins.insert(std::pair<QString,
                     QSharedPointer<CPluginProtocol> >(szProtocol, plugin));
    return 0;
}

int CManagePluginProtocol::UnregisterPlugin(const QString &szProtocol)
{
    std::map<QString, QSharedPointer<CPluginProtocol> >::iterator it;
    it = m_Plugins.find(szProtocol);
    if(m_Plugins.end() == it)
        return 0;

    m_Plugins.erase(szProtocol);
    return 0;
}

QSharedPointer<CPluginProtocol> CManagePluginProtocol::GetPlugin(
        const QString &szProtocol)
{
    std::map<QString, QSharedPointer<CPluginProtocol> >::iterator it;
    it = m_Plugins.find(szProtocol);
    if(m_Plugins.end() == it)
    {
        LOG_MODEL_ERROR("CManagePluginProtocol", "GetPlugin %s is fail",
                        szProtocol.toStdString().c_str());
        return QSharedPointer<CPluginProtocol>();
    }
    return it->second;
}
