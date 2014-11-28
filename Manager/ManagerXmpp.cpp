#include "ManagerXmpp.h"

CManagerXmpp::CManagerXmpp() :
    CManager(),
    m_Client(new CClientXmpp),
    m_User(new CManageUserQXmpp),
    m_GroupChat(new CManageGroupChatQxmpp)
{
    m_Client->SetUser(m_User);
}

QSharedPointer<CClient> CManagerXmpp::GetClient()
{
    return m_Client;
}

QSharedPointer<CManageUser> CManagerXmpp::GetManageUser()
{
    return m_User;
}

QSharedPointer<CManageGroupChat> CManagerXmpp::GetManageGroupChat()
{
    return m_GroupChat;
}
