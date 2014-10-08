#ifndef MANAGER_H
#define MANAGER_H

#include "Client/Client.h"
#include "ManageUser.h"
#include "ManageMessageDialog.h"
#include "Widgets/FrmRecentMessage/ManageRecentMessage.h"
#include <QSharedPointer>

class CManageFileTransfer;
class CManager
{
public:
    CManager();
    virtual ~CManager();
    ///< 协议客户端  
    virtual QSharedPointer<CClient> GetClient() = 0;
    ///< 用户信息管理对象  
    virtual QSharedPointer<CManageUser> GetManageUser() = 0;
    ///< 消息对话框管理对象  
    virtual QSharedPointer<CManageMessageDialog> GetManageMessageDialog();
    virtual QSharedPointer<CManageRecentMessage> GetRecentMessage();
    virtual QSharedPointer<CManageFileTransfer> GetFileTransfer();

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
