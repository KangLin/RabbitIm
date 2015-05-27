#ifndef MANAGECALLXMPP_H
#define MANAGECALLXMPP_H

#include "ManageCall.h"
#include "qxmpp/QXmppCallManager.h"

class CManageCallXmpp : public CManageCall
{
    Q_OBJECT

public:
    explicit CManageCallXmpp(QObject *parent = 0);
    virtual ~CManageCallXmpp();

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

private:
    /**
     * @brief 具体协议实现呼叫  
     * @param szId：用户 Id  
     * @param bVideo：是否包含视频  
     * @return 成功返回0，失败返回非0  
     */
    virtual int OnCall(const QString &szId, bool bVideo = false);

private slots:
    void slotReceived(QXmppCall* pCall);
};

#endif // MANAGECALLXMPP_H
