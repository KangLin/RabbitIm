#ifndef MANAGER_H
#define MANAGER_H

#include "Client/Client.h"
#include "ManageUser.h"
#include "ManageMessageDialog.h"
#include "Widgets/FrmRecentMessage/ManageRecentMessage.h"
#include "Call/ManageCall.h"
#include <QSharedPointer>
#include "Widgets/FrmGroupChat/ManageGroupChat.h"
#include "Plugin/ManagePlugin.h"

class CManageFileTransfer;

/**
 * @defgroup RABBITIM_INTERFACE 接口类模块  
 * @brief 接口类模块  
 * 
 */

/**
 * @defgroup RABBITIM_INTERFACE_MANAGER 管理者接口类模块  
 * @ingroup RABBITIM_INTERFACE
 * @brief 管理者接口类模块  
 */

/**
 * @class CManager
 * @ingroup RABBITIM_INTERFACE_MANAGER RABBITIM_INTERFACE
 * @brief 管理者接口类  
 * @see CClient
 * @see CManageUser
 * @see CManageMessageDialog
 * @see CManageRecentMessage
 * @see CManageFileTransfer
 * @see CManageCall
 * @see CManageGroupChat
 */
class CManager
{
protected:
    CManager();
    virtual ~CManager();

public:
    enum MANAGER_TYPE
    {
        XMPP ///< 用XMPP协议  
    };
private:
    friend class CGlobal;
    /**
     * @brief 生成管理者实例。  
     * 注意：如果要重新生成新的管理者，使用者必须保证原来的管理者已经调用 LogoutClean()、Clean() 。  
     * 本接口不对外开放，只由 CGlobal::GetManager 访问。 
     * @see CGlobal::GetManager  
     * @param type：生成管理者类型  
     * @param bReset：是否重新生成管理者  
     * @return 返回管理者对象  
     */
    static CManager* Instance(MANAGER_TYPE type = XMPP, bool bReset = false);

public:
    ///< 协议客户端  
    virtual QSharedPointer<CClient> GetClient() = 0;
    ///< 用户信息管理对象  
    virtual QSharedPointer<CManageUser> GetManageUser() = 0;
    ///< 消息对话框管理对象  
    virtual QSharedPointer<CManageMessageDialog> GetManageMessageDialog();
    ///< 最近消息管理对象  
    virtual QSharedPointer<CManageRecentMessage> GetRecentMessage();
    ///< 文件传输管理对象  
    virtual QSharedPointer<CManageFileTransfer> GetFileTransfer();
    ///< 呼叫管理对象  
    virtual QSharedPointer<CManageCall> GetCall() = 0;
    ///< 组管理对象  
    virtual QSharedPointer<CManageGroupChat> GetManageGroupChat() = 0;
    ///< 插件管理对象  
    virtual QSharedPointer<CManagePlugin> GetManagePlugins();

    /**
     * 新建用户信息对象  
     * @see CUser
     */
    virtual QSharedPointer<CUserInfo> NewUserInfo() = 0;

    /**
     * @brief 实例初始化  
     * @return 
     */
    virtual int Init();
    /**
     * @brief 实例清理  
     * @return 
     */
    virtual int Clean();

    /**
     * @brief 登录后初始化  
     *
     * @param szId
     * @return int
     */
    virtual int LoginInit(const QString& szId);
    virtual int LogoutClean();
};

#endif // MANAGER_H
