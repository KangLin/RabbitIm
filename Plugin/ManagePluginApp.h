#ifndef CMANAGEPLUGIN_H
#define CMANAGEPLUGIN_H

#include "Manage/Manage.h"
#include "PluginApp.h"
#include <QSharedPointer>
#include <map>
#include <list>
#include <QDir>

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
class RABBITIM_SHARED_LIBRARY CManagePluginApp : public CManage
{
    Q_OBJECT
public:
    explicit CManagePluginApp(QObject *parent = 0);
    virtual ~CManagePluginApp();
    
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
     * @param szId:插件ID  
     * @param plugin：要注册的插件  
     * @return 成功返回0，否则返回非0  
     * @see CPluginApp
     */
    int RegisterPlugin(const QString &szId, CPluginApp* plugin);
    /**
     * @brief 移除插件  
     * @param szId:插件ID  
     * @return 成功返回0，否则返回非0   
     * @see CPluginApp
     */
    int UnregisterPlugin(const QString &szId);
    /**
     * @brief 根据插件名称得到插件  
     * @param szId:插件ID  
     * @return 
     */
    CPluginApp* GetPlugin(const QString &szId);
    /**
     * @brief 得到所有插件  
     * @return 
     */
    std::list<CPluginApp *> GetAllPlugins();

    /**
     * @brief 添加插件到收藏中  
     * @param szId:插件ID  
     * @return 
     */
    int AddFavority(const QString &szId);
    /**
     * @brief 从收藏中删除插件  
     * @param szId:插件ID  
     * @return 
     */
    int RemoveFavority(const QString &szId);
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
    /**增加插件  
    * @param szId:插件ID
    */
    void sigChangedAdd(const QString& szId);
    /**删除插件  
     *  @param szId:插件ID
    */
    void sigChangedRemove(const QString &szId);
    /**增加收藏  
     * @param szId:插件ID
    */
    void sigAddFavority(const QString& szId);
    /**删除收藏  
    * @param szId:插件ID
    */
    void sigRemoveFavority(const QString &szId);

private:
    std::map<QString, CPluginApp* > m_Plugins;
    std::list<QString> m_FavorityPlugins;  //收藏应用  
};

#endif // CMANAGEPLUGIN_H
