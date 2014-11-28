#include "ManageGroupChatQxmpp.h"
#include "Global/Global.h"
#include "Client/ClientXmpp.h"

CManageGroupChatQxmpp::CManageGroupChatQxmpp(QObject *parent) :
    CManageGroupChat(parent)
{
}

int CManageGroupChatQxmpp::Create(const QString &szId, 
                                  const QString &szName,
                                  const QString &szSubject,
                                  const QString szPassword, 
                                  bool bProtracted)
{
    
    return 0;
}

int CManageGroupChatQxmpp::Join(const QString &szId)
{
    QSharedPointer<CClientXmpp> client = (CClientXmpp*)GET_CLIENT.data();
    client->m_MucManager.addRoom(szId);
    return 0;
}

QSharedPointer<CGroupChat> CManageGroupChatQxmpp::Get(const QString &szId)
{
    QMap<QString, QSharedPointer<CGroupChat> >::iterator it;
    it = m_GroupChat.find(szId);
    if(m_GroupChat.end() == it)
        return QSharedPointer<CGroupChat>();
    return it.value();
}
