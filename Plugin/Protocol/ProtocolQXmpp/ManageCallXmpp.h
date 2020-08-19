#ifndef MANAGECALLXMPP_H
#define MANAGECALLXMPP_H

#include "Call/ManageCall.h"
#include "QXmppCallManager.h"


/**
 * @brief CManageCall xmpp 协议实现类  
 * @ingroup RABBITIM_INTERFACE_MANGECALL
 */
class CManageCallXmpp : public CManageCall
{
    Q_OBJECT

public:
    explicit CManageCallXmpp(QObject *parent = nullptr);
    virtual ~CManageCallXmpp();

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

public slots:
    /**
     * @brief 呼叫  
     *
     * @param szId：用户ID  
     * @param bVideo:是否是视频呼叫  
     * @return int
     */
    virtual int Call(const QString &szId, bool bVideo = false);

private:
    /**
     * @brief 具体协议实现呼叫  
     * @param szId：用户 Id  
     * @param bVideo：是否包含视频  
     * @return 成功返回0，失败返回非0  
     */
    virtual int OnCall(const QString &szId, 
                       QSharedPointer<CCallObject> &call,
                       bool bVideo = false);

private slots:
    void slotReceived(QXmppCall* pCall);
};

#endif // MANAGECALLXMPP_H
