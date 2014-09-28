#ifndef MANAGERXMPP_H
#define MANAGERXMPP_H
#include "Manager.h"
#include "Client/ClientXmpp.h"
#include "ManageUserQXmpp.h"

class CManagerXmpp : public CManager
{
public:
    CManagerXmpp();
    
    virtual QSharedPointer<CClient> GetClient();
    virtual QSharedPointer<CManageUser> GetManageUser();
    
private:
    QSharedPointer<CClientXmpp> m_Client;
    QSharedPointer<CManageUserQXmpp> m_User;
};

#endif // MANAGERXMPP_H
