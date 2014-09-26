#ifndef MANAGER_H
#define MANAGER_H

#include "Client/Client.h"
#include "Global/GlobalUser.h"
#include "ManageMessageDialog.h"
#include "ManageMessage.h"
#include <QSharedPointer>

class CManager
{
public:
    CManager();
    virtual ~CManager();
    ///< 协议客户端  
    virtual QSharedPointer<CClient> GetClient() = 0;
    ///< 用户信息管理对象  
    virtual QSharedPointer<CManageUserInfo> GetManageUserInfo() = 0;
    ///< 消息对话框管理对象  
    virtual QSharedPointer<CManageMessageDialog> GetManageMessageDialog();
    ///< 消息管理对象  
    virtual CManageMessage* GetManageMessage();

    /**
     * @brief 登录后初始化  
     *
     * @param szId
     * @return int
     */
    virtual int Init(const QString& szId);
    virtual int Clean();
};

#endif // MANAGER_H
