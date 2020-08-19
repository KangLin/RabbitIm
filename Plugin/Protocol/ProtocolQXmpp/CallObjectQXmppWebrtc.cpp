#include "WebrtcConductor.h"
#undef emit //google 库中定义了此关键字，所以需要undef  

#include "CallObjectQXmppWebrtc.h"
#include "QXmppWebRtcIq.h"
#include "ManageCallWebrtcXmpp.h"
#include "QXmppCallWebrtcManager.h"
#include "qxmpp/QXmppUtils.h"
#include "Global/Global.h"
#include "MainWindow.h"

CCallObjectQXmppWebrtc::CCallObjectQXmppWebrtc(const QString &szJid, bool bVideo,
                          QXmppCallWebrtcManager *pManager,
                                           QObject *parent)
    : CCallObject(QXmppUtils::jidToBareJid(szJid), bVideo, parent),
    m_Conductor(new rtc::RefCountedObject<CWebrtcConductor>(this))
{
    m_szJid = szJid;
    m_Manager = pManager;
    bool check = connect(GET_MAINWINDOW, SIGNAL(sigRefresh()),
                         SLOT(slotUpdateOption()));
    Q_ASSERT(check);
}

CCallObjectQXmppWebrtc::~CCallObjectQXmppWebrtc()
{
    //LOG_MODEL_DEBUG("WEBRTC", "CCallObjectQXmppWebrtc::~CCallObjectQXmppWebrtc()");
    GET_MAINWINDOW->disconnect(this);
}

int CCallObjectQXmppWebrtc::Call()
{
    CCallObject::Call();
    QXmppWebRtcIq iq;
    iq.setTo(m_szJid);
    iq.setType(QXmppIq::Set);
    iq.SetAction(QXmppWebRtcIq::Call);
    iq.SetVideo(IsVideo());
    return m_Manager->sendPacket(iq);
}

int CCallObjectQXmppWebrtc::Accept()
{
    int nRet = 0;
    slotChanageState(CCallObject::ConnectingState);
    nRet = m_Conductor->PeerConnect();
    //发送接收请求  
    QXmppWebRtcIq iq;
    iq.setTo(m_szJid);
    iq.setType(QXmppIq::Set);
    iq.SetAction(QXmppWebRtcIq::Accept);
    return m_Manager->sendPacket(iq);
}

int CCallObjectQXmppWebrtc::ReciveAccept()
{
    int nRet = 0;
    slotChanageState(CCallObject::ConnectingState);
    return nRet;
}

int CCallObjectQXmppWebrtc::Stop(StopState state)
{
    int nRet = 0;
    //发送停止请求  
    QXmppWebRtcIq iq;
    iq.setTo(m_szJid);
    iq.setType(QXmppIq::Set);
    iq.SetAction(QXmppWebRtcIq::Stop);
    nRet = m_Manager->sendPacket(iq);
    //@see CManageCall::slotRosterStatusChanged
    //if(nRet)
    //    return nRet;
    
    nRet = ReciveStop();
    return nRet;
}

int CCallObjectQXmppWebrtc::ReciveStop()
{
    int nRet = 0;
    nRet = m_Conductor->PeerStop();
    slotChanageState(CCallObject::FinishedState);
    return nRet;
}

int CCallObjectQXmppWebrtc::SendSessionDescription(QString szSdp, QString szType, bool bSuccess)
{
    LOG_MODEL_DEBUG("WEBRTC", "SendSessionDescription:%s", szSdp.toStdString().c_str());
    
    QXmppWebRtcIq iq;
    iq.setTo(m_szJid);
    iq.setType(QXmppIq::Set);
    iq.SetAction(QXmppWebRtcIq::DescriptionInfo);
    iq.SetSessionDescription(szSdp, szType, bSuccess);
    return m_Manager->sendPacket(iq);
}

int CCallObjectQXmppWebrtc::ReciveSeesionDescription(QXmppWebRtcIq &iq)
{
    LOG_MODEL_DEBUG("WEBRTC", " CCallObjectQXmppWebrtc::ReciveSeesionDescription");
    QString szSdp, szType;
    bool bSuccess = false;
    if(iq.GetSessionDescription(szSdp, szType, bSuccess))
    {
        LOG_MODEL_ERROR("WEBRTC", "iq.GetSessionDescription fail");
        return -1;
    }
    //判断是否出错，如果出错，则需要做些清理  
    if(bSuccess)
        return m_Conductor->AcceptConnect(szSdp.toStdString(), szType.toStdString());
    else
        return this->ReciveStop();
}

int CCallObjectQXmppWebrtc::SendTransportInfo(QString sdp_mid, int sdp_mline_index, QString sdp)
{
    LOG_MODEL_INFO("WEBRTC", "SendTransportInfo:mid:%s;index:%d;sdp:%s",
                    sdp_mid.toStdString().c_str(), sdp_mline_index, sdp.toStdString().c_str());
    QXmppWebRtcIq iq;
    iq.setTo(m_szJid);
    iq.setType(QXmppIq::Set);
    iq.SetAction(QXmppWebRtcIq::TransportInfo);
    iq.SetTransportInfo(sdp_mid, sdp_mline_index, sdp);
    return m_Manager->sendPacket(iq);
}

int CCallObjectQXmppWebrtc::ReciveTransportInfo(QXmppWebRtcIq& iq)
{
    QString sdp_mid;
    int sdp_mline_index;
    QString sdp;
    iq.GetTransportInfo(sdp_mid, sdp_mline_index, sdp);
    LOG_MODEL_INFO("WEBRTC", "ReciveTransportInfo:mid:%s;index:%d;sdp:%s",
                    sdp_mid.toStdString().c_str(), sdp_mline_index, sdp.toStdString().c_str());
    
    return m_Conductor->ReciveIceCandidate(sdp_mid.toStdString(),
                                           sdp_mline_index,
                                           sdp.toStdString());
}

void CCallObjectQXmppWebrtc::slotUpdateOption()
{
    if(CGlobal::Instance()->GetIsShowLocaleVideo())
    {
        m_Conductor->CloseLocaleRander(false);
    } else {
        m_Conductor->CloseLocaleRander(true);
    }
}
