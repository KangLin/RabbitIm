#include "PluginProtocolQXmpp.h"
#include "UserInfoXmpp.h"
#ifdef RABBITIM_USE_QXMPP_CALL
    #ifdef RABBITIM_USE_WEBRTC
        #include "ManageCallWebrtcXmpp.h"
    #else
        #include "ManageCallXmpp.h"
    #endif
#else
    #include "Call/ManageCall.h"
#endif

CPluginProtocolQXMPP::CPluginProtocolQXMPP(QObject *parent) :
    QObject(parent),
    CPluginProtocol(),
    m_Client(new CClientXmpp),
    m_User(new CManageUserQXmpp),
    m_GroupChat(new CManageGroupChatQxmpp),
    #ifdef RABBITIM_USE_QXMPP_CALL
        #ifdef RABBITIM_USE_WEBRTC
            m_Call(new CManageCallWebrtcXmpp)
        #else
            m_Call(new CManageCallXmpp)
        #endif
    #else
        m_Call(new CManageCall)
    #endif
{
    m_Client->SetUser(m_User);
}

QString CPluginProtocolQXMPP::ID()
{
    return "ProtocolQXmpp";
}

QString CPluginProtocolQXMPP::Name()
{
    return "QXMPP";
}

QSharedPointer<CClient> CPluginProtocolQXMPP::GetClient()
{
    return m_Client;
}

QSharedPointer<CManageUser> CPluginProtocolQXMPP::GetManageUser()
{
    return m_User;
}

QSharedPointer<CManageCall> CPluginProtocolQXMPP::GetCall()
{
    return m_Call;
}

QSharedPointer<CManageGroupChat> CPluginProtocolQXMPP::GetManageGroupChat()
{
    return m_GroupChat;
}

QSharedPointer<CUserInfo> CPluginProtocolQXMPP::NewUserInfo()
{
    return QSharedPointer<CUserInfo>(new CUserInfoXmpp);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ProtocolQXmpp, CPluginProtocolQXMPP)
#endif // QT_VERSION < 0x050000
