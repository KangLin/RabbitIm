#include "GroupChatQxmpp.h"
#include "Global/Global.h"
#include "ManageGroupChatQxmpp.h"
#include "qxmpp/QXmppUtils.h"
#include "qxmpp/QXmppMessage.h"

CGroupChatQxmpp::CGroupChatQxmpp(QXmppMucRoom *pRoom, QObject *parent) :
    CGroupChat(parent)
{
    m_pRoom = pRoom;
    bool check = connect(pRoom, 
                         SIGNAL(joined()),
                         this,
                         SLOT(slotJoined()));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(allowedActionsChanged(QXmppMucRoom::Actions)),
            SLOT(slotAllowedActionsChanged(QXmppMucRoom::Actions)));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(configurationReceived(QXmppDataForm)),
                    SLOT(slotConfigurationReceived(QXmppDataForm)));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(error(QXmppStanza::Error)),
                    SLOT(slotError(QXmppStanza::Error)));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(kicked(QString,QString)),
                    SLOT(slotKicked(QString,QString)));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(left()),
                    SLOT(slotLeft()));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(messageReceived(QXmppMessage)),
                    SLOT(slotMessageReceived(QXmppMessage)));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(nameChanged(QString)),
                    SLOT(slotNameChanged(QString)));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(nickNameChanged(QString)),
                    SLOT(slotNickNameChanged(QString)));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(participantAdded(QString)),
                    SLOT(slotParticipantAdded(QString)));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(participantRemoved(QString)),
                    SLOT(slotParticipantRemoved(QString)));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(participantChanged(QString)),
                    SLOT(slotParticipantChanged(QString)));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(participantsChanged()),
                    SLOT(slotParticipantsChanged()));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(permissionsReceived(QList<QXmppMucItem>)),
                    SLOT(slotPermissionsReceived(QList<QXmppMucItem>)));
    Q_ASSERT(check);
    
    check = connect(pRoom, SIGNAL(subjectChanged(QString)),
                    SLOT(slotSubjectChanged(QString)));
    Q_ASSERT(check);
}

void CGroupChatQxmpp::slotJoined()
{
    emit GETMANAGER->GetManageGroupChat()->sigJoined(Id());
}

void CGroupChatQxmpp::slotAllowedActionsChanged(QXmppMucRoom::Actions actions)
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotAllowedActionsChanged:%d", actions);
}

void CGroupChatQxmpp::slotConfigurationReceived(const QXmppDataForm &configuration)
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotConfigurationReceived");
}

void CGroupChatQxmpp::slotError(const QXmppStanza::Error &error)
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotError");
}

void CGroupChatQxmpp::slotKicked(const QString &jid, const QString &reason)
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotKicked:%s", qPrintable(jid));
}

void CGroupChatQxmpp::slotIsJoinedChanged()
{
     LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotIsJoinedChanged");
}

void CGroupChatQxmpp::slotLeft()
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotLeft");
    GETMANAGER->GetManageGroupChat()->slotLeave(Id());
}

void CGroupChatQxmpp::slotMessageReceived(const QXmppMessage &message)
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotMessageReceived:%s", message.body().toStdString().c_str());
}

void CGroupChatQxmpp::slotNameChanged(const QString &name)
{
     LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotNameChanged:%s", qPrintable(name));
}

void CGroupChatQxmpp::slotNickNameChanged(const QString &nickName)
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotNickNameChanged:%s", qPrintable(nickName));
}

void CGroupChatQxmpp::slotParticipantAdded(const QString &jid)
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotParticipantAdded:%s", qPrintable(jid));
}

void CGroupChatQxmpp::slotParticipantChanged(const QString &jid)
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotParticipantChanged:%s", qPrintable(jid));
}

void CGroupChatQxmpp::slotParticipantRemoved(const QString &jid)
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotParticipantRemoved:%s", qPrintable(jid));
}

void CGroupChatQxmpp::slotParticipantsChanged()
{
     LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotParticipantsChanged");
}

void CGroupChatQxmpp::slotPermissionsReceived(const QList<QXmppMucItem> &permissions)
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotPermissionsReceived");
}

void CGroupChatQxmpp::slotSubjectChanged(const QString &subject)
{
    LOG_MODEL_DEBUG("CGroupChatQxmpp", "CGroupChatQxmpp::slotSubjectChanged:%s", qPrintable(subject));
}

QString CGroupChatQxmpp::Id()
{
    RABBITIM_ASSERT(m_pRoom, QString());
    return m_pRoom->jid();
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
    RABBITIM_ASSERT(m_pRoom, QString());
    return m_pRoom->nickName();
}

QString CGroupChatQxmpp::Subject()
{
    RABBITIM_ASSERT(m_pRoom, QString());
    return m_pRoom->subject();
}

int CGroupChatQxmpp::Leave()
{
    RABBITIM_ASSERT(m_pRoom, -1);
    m_pRoom->leave();
    return 0;
}
