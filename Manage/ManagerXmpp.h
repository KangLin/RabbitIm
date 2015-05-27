#ifndef MANAGERXMPP_H
#define MANAGERXMPP_H
#include "Manager.h"
#include "Client/ClientXmpp.h"
#include "ManageUserQXmpp.h"
#include "Widgets/FrmGroupChat/ManageGroupChatQxmpp.h"

/**
 * @defgroup RABBITIM_IMPLEMENT_QXMPP  接口类模块  
 * @brief 接口类模块  
 * 
 */

/**
 * @defgroup RABBITIM_IMPLEMENT_QXMPP_MANAGER 管理者接口类模块  
 * @ingroup RABBITIM_IMPLEMENT_QXMPP
 * @brief 管理者接口类模块  
 */

/**
 * @brief 管理者接口xmpp实现类
 * @ingroup RABBITIM_IMPLEMENT_QXMPP_MANAGER
 */
class CManagerXmpp : public CManager
{
public:
    CManagerXmpp();
    
    virtual QSharedPointer<CClient> GetClient();
    virtual QSharedPointer<CManageUser> GetManageUser();
    virtual QSharedPointer<CManageGroupChat> GetManageGroupChat();
    ///< 呼叫管理对象  
    virtual QSharedPointer<CManageCall> GetCall();

    ///< 新建用户信息对象  
    virtual QSharedPointer<CUserInfo> NewUserInfo();

private:
    QSharedPointer<CClientXmpp> m_Client;
    QSharedPointer<CManageUserQXmpp> m_User;
    QSharedPointer<CManageGroupChatQxmpp> m_GroupChat;
    QSharedPointer<CManageCall> m_Call;
};

#endif // MANAGERXMPP_H
