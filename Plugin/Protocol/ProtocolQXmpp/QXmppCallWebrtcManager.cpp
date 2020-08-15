#include "QXmppCallWebrtcManager.h"
#include "qxmpp/QXmppClient.h"
#include "ManageCallWebrtcXmpp.h"
#include "Global/Log.h"

QXmppCallWebrtcManager::QXmppCallWebrtcManager(CManageCallWebrtcXmpp *pManager) : QXmppClientExtension()
{
    m_pManager = pManager;
}

int QXmppCallWebrtcManager::sendPacket(QXmppWebRtcIq &iq)
{
    if(this->client()->sendPacket(iq))
        return 0;
    LOG_MODEL_ERROR("WEBRTC", "QXmppCallWebrtcManager::sendPacket");
    return -1;
}

QStringList QXmppCallWebrtcManager::discoveryFeatures() const
{
    return QStringList() << ns_webrtc;
}

bool QXmppCallWebrtcManager::handleStanza(const QDomElement &element)
{
    if(element.tagName() == "iq")
    {
        if (QXmppWebRtcIq::isWebRtcIq(element))
        {
            QXmppWebRtcIq webrtcIq;
            webrtcIq.parse(element);
            if(webrtcIq.type() != QXmppIq::Set)
            {
                return true;
            }
            SendAck(webrtcIq);
            Q_ASSERT(m_pManager);
            if(m_pManager)
                m_pManager->WebrtcIqReceived(webrtcIq);
            return true;
        }
    }
    return false;
}

int QXmppCallWebrtcManager::SendAck(QXmppWebRtcIq iq)
{
    QXmppIq ack;
    ack.setId(iq.id());
    ack.setTo(iq.from());
    ack.setType(QXmppIq::Result);
    return client()->sendPacket(ack);
}
