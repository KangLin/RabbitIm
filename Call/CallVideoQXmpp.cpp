#include "CallVideoQXmpp.h"
#include "qxmpp/QXmppUtils.h"

CCallVideoQXmpp::CCallVideoQXmpp(QXmppCall* pCall, QObject *parent) : CCallVideo(parent)
{
    m_pQXmppCall = pCall;
    if(pCall)
    {
        SetId(QXmppUtils::jidToBareJid(pCall->jid()));
        ConnectionCallSlot(pCall);
    }
}

int CCallVideoQXmpp::ConnectionCallSlot(QXmppCall *pCall)
{
    return 0;
    //只有主叫方才有的事件  
    bool check = connect(pCall, SIGNAL(ringing()),
                    SLOT(ringing()));
    Q_ASSERT(check);
    
    //以下是双方都有的事件  
    check = connect(pCall, SIGNAL(connected()),
                         SLOT(slotConnection()));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(stateChanged(QXmppCall::State)),
                    SLOT(slotStateChanged(QXmppCall::State)));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(audioModeChanged(QIODevice::OpenMode)),
                    SLOT(slotAudioModeChanged(QIODevice::OpenMode)));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(videoModeChanged(QIODevice::OpenMode)),
                    SLOT(slotVideoModeChanged(QIODevice::OpenMode)));
    Q_ASSERT(check);

    check = connect(pCall, SIGNAL(finished()),
                    SLOT(slotFinished()));
    Q_ASSERT(check);

    if(check)
        return 0;

    return -1;
}

int CCallVideoQXmpp::Accept()
{
    int nRet = 0;
    m_pQXmppCall->accept();
    return nRet;
}

int CCallVideoQXmpp::Cancel()
{
    int nRet = 0;
    m_pQXmppCall->hangup();
    return nRet;
}

CCallObject::State CCallVideoQXmpp::GetState()
{
    return (State)m_pQXmppCall->state();
}
