#ifndef USERINFOROSTER_H
#define USERINFOROSTER_H

#include "UserInfo.h"
#include <QSet>
#include "qxmpp/QXmppRosterIq.h"

class CUserInfoRoster : public CUserInfo
{
    Q_OBJECT
public:
    CUserInfoRoster(QObject *parent = NULL);
    QSet<QString> GetGroup();

#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator <<(QDataStream &output, const CUserInfoRoster &roster);
    friend QDataStream & operator >>(QDataStream &input, CUserInfoRoster &roster);
#endif

    virtual int UpdateUserInfo(const QXmppRosterIq::Item &rosterItem);
    virtual int UpdateUserInfo(const QXmppVCardIq &vCard, QString jid);

    virtual QXmppPresence::AvailableStatusType GetStatus();//得到本地用户状态  
    virtual int SetStatus(QXmppPresence::AvailableStatusType status);

private:
    QSet<QString> m_Group;
    QXmppRosterIq::Item::SubscriptionType m_subscriptionType;
    QXmppPresence::AvailableStatusType m_LocalStatus;//本地用户的状态  
};

#endif // USERINFOROSTER_H
