#ifndef MANAGER_H
#define MANAGER_H

#include "Client/Client.h"
#include "ManageUser.h"
#include "ManageMessageDialog.h"
#include "ManageMessageDialogBigScreen.h"
#include "Widgets/FrmRecentMessage/ManageRecentMessage.h"
#include "Call/ManageCall.h"
#include <QSharedPointer>
#include "Widgets/FrmGroupChat/ManageGroupChat.h"
#include "Plugin/ManagePluginApp.h"
#include "Plugin/ManagePluginProtocol.h"

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
 * @see CManagePlugin
 */
class RABBITIM_SHARED_LIBRARY CManager
{
public:
    CManager();
    virtual ~CManager();

public:
    ///< 协议客户端  
    virtual QSharedPointer<CClient> GetClient();
    ///< 用户信息管理对象  
    virtual QSharedPointer<CManageUser> GetManageUser();
    ///< 消息对话框管理对象  
    virtual QSharedPointer<CManageMessageDialog> GetManageMessageDialog();
    ///< 最近消息管理对象  
    virtual QSharedPointer<CManageRecentMessage> GetRecentMessage();
    ///< 文件传输管理对象  
    virtual QSharedPointer<CManageFileTransfer> GetFileTransfer();
    ///< 呼叫管理对象  
    virtual QSharedPointer<CManageCall> GetCall();
    ///< 组管理对象  
    virtual QSharedPointer<CManageGroupChat> GetManageGroupChat();
    ///< 插件管理对象  
    virtual QSharedPointer<CManagePluginApp> GetManagePluginApp();
    virtual QSharedPointer<CManagePluginProtocol> GetManagePluginProtocol();
    
    ///< 更改通信协议  
    virtual int ChangeProtolcol(QString szProtocol);
    
    /**
     * 新建用户信息对象  
     * @see CUser
     */
    virtual QSharedPointer<CUserInfo> NewUserInfo();

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
     * @see CClient::slotClientConnected
     */
    virtual int LoginInit(const QString& szId);
    /**
     * @brief 登出后清理操作  
     * @return 
     * @see CClient::slotClientDisconnected
     */
    virtual int LogoutClean();

private:
    CPluginProtocol* m_PluginProtocol;
    QSharedPointer<CManagePluginProtocol> m_ManagePluginProtocol;
    QSharedPointer<CManagePluginApp> m_ManagePluginApp;
    QSharedPointer<CManageFileTransfer> m_ManageFile;
    QSharedPointer<CManageRecentMessage> m_ManageRecentMessage;
    QSharedPointer<CManageMessageDialog> n_ManageMessageDialog;

    //查找插件
    int FindPlugins(QDir dir);
};

#endif // MANAGER_H
