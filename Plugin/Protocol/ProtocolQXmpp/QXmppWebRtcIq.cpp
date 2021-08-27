#include "QXmppWebRtcIq.h"
#include "RabbitCommonLog.h"

const char* ns_webrtc = "urn:xmpp:rabbitim:webrtc";
const char* g_szAction[] = {
    "Call",
    "Accept",
    "Stop",
    "DescriptionInfo",
    "TransportInfo"
};

QXmppWebRtcIq::QXmppWebRtcIq()
{
    m_Action = None;
    m_bVideo = false;
    m_nMlineIndex = 0;
    m_nSessionDescription = 0;
}

bool QXmppWebRtcIq::isWebRtcIq(const QDomElement &element)
{
    QDomElement domElement = element.firstChildElement("webrtc");
    return (domElement.namespaceURI() == ns_webrtc);
}

void QXmppWebRtcIq::parseElementFromChild(const QDomElement &element)
{
    QDomElement webrtcElement = element.firstChildElement("webrtc");
    if(webrtcElement.isNull())
    {
        LOG_MODEL_ERROR("QXmppWebRtcIq", "webrtcElement is null");
        return;
    }
    QString szAction = webrtcElement.attribute("action");
    for(int i = Call; i <= TransportInfo; i++)
    {
        if(g_szAction[i] == szAction)
        {
            m_Action = (ACTION)i;
            break;
        }
    }
    if(webrtcElement.hasAttribute("video"))
        m_bVideo = webrtcElement.attribute("video").toInt() ? true : false;
    QDomElement childElement = webrtcElement.firstChildElement(g_szAction[m_Action]);
    if(childElement.isNull())
        return;
    QString value;
    value = childElement.text();
    switch(m_Action){
    case DescriptionInfo:
        if(childElement.hasAttribute("success"))
            m_nSessionDescription = childElement.attribute("success").toInt();
        if(childElement.hasAttribute("type"))
            m_szType = childElement.attribute("type");
        m_szSessionDescription = value;
        break;
    case TransportInfo:
        if(childElement.hasAttribute("mid"))
            m_szMid = childElement.attribute("mid");
        if(childElement.hasAttribute("mline_index"))
            m_nMlineIndex = childElement.attribute("mline_index").toInt();
        m_szTransportInfo = value;
        break;
    }
}

void QXmppWebRtcIq::toXmlElementFromChild(QXmlStreamWriter *writer) const
{
    writer->writeStartElement("webrtc");
    writer->writeAttribute("xmlns", ns_webrtc);
    writer->writeAttribute("action", g_szAction[m_Action]);
    if(m_bVideo)
        writer->writeAttribute("video", "1");
    
    switch (m_Action) {
    case DescriptionInfo:
        writer->writeStartElement(g_szAction[m_Action]);
        writer->writeAttribute("success", QString::number(m_nSessionDescription));
        writer->writeAttribute("type", m_szType);
        writer->writeCharacters(m_szSessionDescription);
        writer->writeEndElement();
        break;
    case TransportInfo:
        writer->writeStartElement(g_szAction[m_Action]);
        writer->writeAttribute("mid", m_szMid);
        writer->writeAttribute("mline_index", QString::number(m_nMlineIndex));
        writer->writeCharacters(m_szTransportInfo);
        writer->writeEndElement();
        break;
    default:
        break;
    }
    
    writer->writeEndElement();

}

QXmppWebRtcIq::ACTION QXmppWebRtcIq::GetAction()
{
    return m_Action;
}

int QXmppWebRtcIq::SetAction(ACTION action)
{
    m_Action = action;
    return 0;
}

int QXmppWebRtcIq::SetVideo(bool video)
{
    m_bVideo = video;
    return 0;
}

bool QXmppWebRtcIq::IsVideo()
{
    return m_bVideo;
}

int QXmppWebRtcIq::SetSessionDescription(QString szSdp, QString szType, bool bSuccess)
{
    int nRet = 0;
    m_Action = DescriptionInfo;
    m_szType = szType;
    m_szSessionDescription = szSdp;
    m_nSessionDescription = bSuccess ? 1 : 0;
    return nRet;
}

int QXmppWebRtcIq::GetSessionDescription(QString &szSdp, QString &szType, bool &bSuccess)
{
    bSuccess = m_nSessionDescription ? true : false;
    szSdp = m_szSessionDescription;
    szType = m_szType;
    return 0;
}

int QXmppWebRtcIq::SetTransportInfo(QString szSdpMid, int nIndex, QString szSdp)
{
    m_Action = TransportInfo;
    m_szTransportInfo = szSdp;
    m_nMlineIndex = nIndex;
    m_szMid = szSdpMid;
    return 0;
}

int QXmppWebRtcIq::GetTransportInfo(QString &szSdpMid, int &nIndex, QString &szSdp)
{
    szSdpMid = m_szMid;
    nIndex = m_nMlineIndex;
    szSdp = m_szTransportInfo;
    return 0;
}
