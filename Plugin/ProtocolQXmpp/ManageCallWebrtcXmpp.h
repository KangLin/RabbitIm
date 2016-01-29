#ifndef CMANAGECALLWEBRTCXMPP_H
#define CMANAGECALLWEBRTCXMPP_H

#include "ManageCallXmpp.h"
#include "CallObjectQXmppWebrtc.h"
#include "QXmppWebRtcIq.h"

/**
 * @brief CManageCall webrtc xmpp 协议实现类  
 * @ingroup RABBITIM_INTERFACE_MANGECALL
 */
class CManageCallWebrtcXmpp : public CManageCallXmpp
{
    Q_OBJECT
public:
    CManageCallWebrtcXmpp(QObject *parent = 0);
    
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

    /**
     * @brief 接收处理 webrtc xmpp iq 消息  
     * @param iq
     * @return 
     */
    int WebrtcIqReceived(QXmppWebRtcIq &iq);
    //处理接收到 call 消息  
    virtual int OnReciveCall(QXmppWebRtcIq &iq);
    friend class QXmppCallWebrtcManager;
};

#endif // CMANAGECALLWEBRTCXMPP_H
