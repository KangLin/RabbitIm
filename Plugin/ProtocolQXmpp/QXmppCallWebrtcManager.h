#ifndef QXMPPCALLWEBRTCEXTENSION_H
#define QXMPPCALLWEBRTCEXTENSION_H

#include "QXmppWebRtcIq.h"
#include "qxmpp/QXmppClientExtension.h"

class CManageCallWebrtcXmpp;
/**
 * @brief 处理 webrtc xmpp 协议实现类  
 */
class QXmppCallWebrtcManager : public QXmppClientExtension
{
    Q_OBJECT
public:
    QXmppCallWebrtcManager(CManageCallWebrtcXmpp* pManager);
    virtual QStringList discoveryFeatures() const;
    virtual bool handleStanza(const QDomElement &element);
    virtual int sendPacket(QXmppWebRtcIq &iq);
    
private:
    CManageCallWebrtcXmpp* m_pManager;
    int SendAck(QXmppWebRtcIq iq);
};

#endif // QXMPPCALLWEBRTCEXTENSION_H
