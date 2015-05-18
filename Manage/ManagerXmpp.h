#ifndef MANAGERXMPP_H
#define MANAGERXMPP_H
#include "Manager.h"
#include "Client/ClientXmpp.h"
#include "ManageUserQXmpp.h"
#include "Widgets/FrmGroupChat/ManageGroupChatQxmpp.h"

class CManagerXmpp : public CManager
{
public:
    CManagerXmpp();
    
    virtual QSharedPointer<CClient> GetClient();
    virtual QSharedPointer<CManageUser> GetManageUser();
    virtual QSharedPointer<CManageGroupChat> GetManageGroupChat();
    
private:
    QSharedPointer<CClientXmpp> m_Client;
    QSharedPointer<CManageUserQXmpp> m_User;
    QSharedPointer<CManageGroupChatQxmpp> m_GroupChat;
};

#endif // MANAGERXMPP_H
