#include "GroupChatQxmpp.h"
#include "Global/Global.h"
#include "ManageGroupChatQxmpp.h"
#include "qxmpp/QXmppUtils.h"

CGroupChatQxmpp::CGroupChatQxmpp(QXmppMucRoom *pRoom, QObject *parent) :
    CGroupChat(parent)
{
    m_pRoom = pRoom;
    bool check = connect(pRoom, 
                         SIGNAL(joined()),
                         this,
                         SLOT(slotJoined()));
    Q_ASSERT(check);
}

void CGroupChatQxmpp::slotJoined()
{
    emit GETMANAGER->GetManageGroupChat()->sigJoined(Id());
}

QString CGroupChatQxmpp::Id()
{
    if(m_pRoom)
        return m_pRoom->jid();
    return QString();
}

QString CGroupChatQxmpp::ShowName()
{
    QString szName;
    if(m_pRoom)
    {
        szName = m_pRoom->name();
        LOG_MODEL_DEBUG("CGroupChatQxmpp", "name:%s", m_pRoom->name().toStdString().c_str());
        if(szName.isEmpty())
        {
            szName = QXmppUtils::jidToUser(m_pRoom->jid());
        }
    }
    return szName;
}

QString CGroupChatQxmpp::Nick()
{
    if(m_pRoom)
        return m_pRoom->nickName();
    return QString();
}

int CGroupChatQxmpp::Leave()
{
    if(!m_pRoom)
        return -1;
    m_pRoom->left();
    return 0;
}
