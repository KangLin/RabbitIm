#ifndef QXMPPWEBRTCIQ_H
#define QXMPPWEBRTCIQ_H

#include "qxmpp/QXmppIq.h"
#include <QDomElement>

extern const char* ns_webrtc;
class QXmppWebRtcIq : public QXmppIq
{
public:
    QXmppWebRtcIq();
    /// This enum is used to describe a webrtc action.
    enum ACTION {
        Call = 0,
        Accept,
        Stop,
        DescriptionInfo,
        TransportInfo,
        None
    };
    
    /// \cond
    static bool isWebRtcIq(const QDomElement &element);
    /// \endcond
    int SetAction(ACTION action);
    ACTION GetAction();
    bool IsVideo();
    int SetVideo(bool video);
    int SetSessionDescription(QString szSdp, QString szType, bool bSuccess);
    int GetSessionDescription(QString &szSdp, QString &szType, bool &bSuccess);
    int SetTransportInfo(QString szSdpMid, int nIndex, QString szSdp);
    int GetTransportInfo(QString &szSdpMid, int &nIndex, QString &szSdp);

protected:
    /// \cond
    void parseElementFromChild(const QDomElement &element);
    void toXmlElementFromChild(QXmlStreamWriter *writer) const;
    /// \endcond
private:
    ACTION m_Action;
    bool m_bVideo;
    
    QString m_szType;
    QString m_szSessionDescription;
    int m_nSessionDescription;
    
    QString m_szMid;
    int m_nMlineIndex;
    QString m_szTransportInfo;
};

#endif // QXMPPWEBRTCIQ_H
