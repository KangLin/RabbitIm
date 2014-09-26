#ifndef MANAGERXMPP_H
#define MANAGERXMPP_H
#include "Manager.h"
#include "Client/ClientXmpp.h"
#include "Global/GlobalUserQXmpp.h"

class CManagerXmpp : public CManager
{
public:
    CManagerXmpp();
    
    virtual QSharedPointer<CClient> GetClient();
    virtual QSharedPointer<CManageUserInfo> GetManageUserInfo();
    
private:
    QSharedPointer<CClientXmpp> m_Client;
    QSharedPointer<CManageUserInfoQXmpp> m_User;
};

#endif // MANAGERXMPP_H
