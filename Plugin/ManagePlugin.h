#ifndef CMANAGEPLUGIN_H
#define CMANAGEPLUGIN_H

#include "Manage/Manage.h"
#include "PluginApp.h"
#include <QSharedPointer>
#include <map>
#include <list>

/**
  * @defgroup RABBITIM_INTERFACE_MANAGEPLUGIN 插件管理类模块  
  * @ingroup RABBITIM_INTERFACE_MANAGER
  * @brief 插件管理类模块，用于管理插件  
  */

/**
 * @class CManagePlugin
 * @ingroup RABBITIM_INTERFACE_MANAGEPLUGIN RABBITIM_INTERFACE
 * @brief 插件管理类  
 * @see CManager
 */
class CManagePlugin : public CManage
{
    Q_OBJECT
public:
    explicit CManagePlugin(QObject *parent = 0);
    /**
     * @brief 用户登录成功后调用,用于初始化工作  
     *
     * @param szId:登录用户名  
     * @return int
     */
    virtual int Init(const QString &szId);
    /**
     * @brief 用户登出时调用,用于清理工作   
     *
     * @return int
     */
    virtual int Clean();

    /**
     * @brief 注册插件,插件必须在用户登录前进行注册,否则不会调用插件的 Init() 进行初始化    
     * @param plugin：要注册的插件  
     * @return 成功返回0，否则返回非0  
     * @see CPluginApp
     */
    int RegisterPlugin(const QString &szName,
                       QSharedPointer<CPluginApp> plugin);
    /**
     * @brief 移除插件  
     * @param plugin：要移除的插件  
     * @return 成功返回0，否则返回非0   
     * @see CPluginApp
     */
    int UnregisterPlugin(const QString &szName);
    /**
     * @brief 根据插件名称得到插件  
     * @param 插件名称  
     * @return 
     */
    QSharedPointer<CPluginApp> GetPlugin(const QString &szName);
    /**
     * @brief 得到所有插件  
     * @return 
     */
    std::list<QSharedPointer<CPluginApp> > GetAllPlugins();

    /**
     * @brief 添加插件到收藏中  
     * @param plugin
     * @return 
     */
    int AddFavority(const QString &plugin);
    /**
     * @brief 从收藏中删除插件  
     * @param plugin
     * @return 
     */
    int RemoveFavority(const QString &plugin);
    /**
     * @brief 得到所有收藏  
     * @return 
     */
    std::list<QString> GetFavorites();
private:
    /**
     * @brief 从存储中加载信息  
     *
     * @param szLocaleJid  
     * @return int  
     */
    int LoadFromStorage(const QString &szId);
    /**
     * @brief 保存信息到存储  
     *
     * @return int  
     */
    int SaveToStorage();

public:
signals:
    //增加插件  
    void sigChangedAdd(const QString& szName);
    //删除插件  
    void sigChangedRemove(const QString &szName);
    //增加收藏  
    void sigAddFavority(const QString& szName);
    //删除收藏  
    void sigRemoveFavority(const QString &szName);

private:
    std::map<QString, QSharedPointer<CPluginApp> > m_Plugins;
    std::list<QString> m_FavorityPlugins;
};

#endif // CMANAGEPLUGIN_H
