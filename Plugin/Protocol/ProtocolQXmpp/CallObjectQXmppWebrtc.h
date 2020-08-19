#ifndef CCALLOBJECTQXMPPWEBRTC_H
#define CCALLOBJECTQXMPPWEBRTC_H

#include <QObject>
#include <QVideoFrame>
#include <QSharedPointer>
#include <QTimer>
#include "Call/CallObject.h"

class CWebrtcConductor;
class QXmppCallWebrtcManager;
class QXmppWebRtcIq;
#include "webrtc/base/scoped_ref_ptr.h"

/**
 * @brief webrtc呼叫qxmpp实现类  
 * @ingroup RABBITIM_IMPLEMENT_QXMPP
 * 
 * 应用发出的指令  
 *    caller                  callee
 *      |                       |
 *      |         call          |
 *      |---------------------->|
 *      |                       |
 *      |         ring          |
 *      |<----------------------| 
 *      |                       |
 *      |        accept         |
 *      |<----------------------|
 *      |                       |
 *      |         stop          |
 *      |<--------------------->|
 */
class CCallObjectQXmppWebrtc : public CCallObject
{
    Q_OBJECT

public:
    explicit CCallObjectQXmppWebrtc(const QString &szJid,
                                    bool bVideo = false,
                QXmppCallWebrtcManager* pManager = NULL,
                                   QObject *parent = 0);
    virtual ~CCallObjectQXmppWebrtc();
    
public slots:
    virtual int Call();
    /**
     * 停止呼叫  
     * 实现协议接收到远程的呼叫信令后，  
     * 如果不接收对方呼叫，则调用此方法  
     * 如果通话结束，也调用此方法  
     */
    virtual int Stop(StopState state = Reject);
    virtual int ReciveStop();
    /**
     * 接收呼叫  
     * 实现协议接收到远程的呼叫信令后，  
     * 如果接受，则调用此方法。  
     */
    virtual int Accept();
    virtual int ReciveAccept();
    
    virtual int SendSessionDescription(QString szSdp, QString szType, bool bSuccess = true);
    virtual int ReciveSeesionDescription(QXmppWebRtcIq &iq);
    
    virtual int SendTransportInfo(QString sdp_mid, int sdp_mline_index, QString sdp);
    virtual int ReciveTransportInfo(QXmppWebRtcIq& iq);
    
private slots:
    //配置更新  
    void slotUpdateOption();

private:
    QXmppCallWebrtcManager* m_Manager;
    QString m_szJid;
    rtc::scoped_refptr<CWebrtcConductor> m_Conductor;
    friend class CWebrtcConductor;
};

#endif // CCALLOBJECTQXMPPWEBRTC_H
