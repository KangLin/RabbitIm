#ifndef CPLUGINPROTOCOLMANAGER_H
#define CPLUGINPROTOCOLMANAGER_H

#include "Manage/Manage.h"
#include "PluginProtocol.h"
#include <QDir>

class CManagePluginProtocol : public CManage
{
    Q_OBJECT
public:
    explicit CManagePluginProtocol(QObject *parent = 0);
    /**
     * @brief 用户登录成功后调用,用于初始化工作
     *
     * @param szId:登录用户名
     * @return int
     */
    virtual int LoginInit(const QString &szId);
    /**
     * @brief 用户登出时调用,用于清理工作
     *
     * @return int
     */
    virtual int LogoutClean();

    /**
     * @brief 注册插件,插件必须在用户登录前进行注册,否则不会调用插件的 LoginInit() 进行初始化
     * @param szProtocol:插件ID
     * @param plugin：要注册的插件
     * @return 成功返回0，否则返回非0
     * @see CPluginApp
     */
    int RegisterPlugin(const QString &szProtocol, CPluginProtocol* plugin);
    /**
     * @brief 移除插件
     * @param szProtocol:插件ID
     * @return 成功返回0，否则返回非0
     * @see CPluginApp
     */
    int UnregisterPlugin(const QString &szProtocol);
    /**
     * @brief 根据插件名称得到插件
     * @param szProtocol:插件ID
     * @return
     */
    CPluginProtocol* GetPlugin(const QString &szProtocol);
    /**
     * @brief 得到所有插件  
     * @return 
     */
    std::list<CPluginProtocol *> GetAllPlugins();
private:
    std::map<QString, CPluginProtocol* > m_Plugins;
};

#endif // CPLUGINPROTOCOLMANAGER_H
