#include "ManagerXmpp.h"

CManagerXmpp::CManagerXmpp() :
    m_Client(new CClientXmpp),
    m_User(new CGlobalUserQXmpp)
{
    m_Client->SetUser(m_User);
}

QSharedPointer<CClient> CManagerXmpp::GetClient()
{
    return m_Client;
}

QSharedPointer<CGlobalUser> CManagerXmpp::GetGlobalUser()
{
    return m_User;
}
