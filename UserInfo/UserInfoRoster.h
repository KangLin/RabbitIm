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
    QSet<QString> &GetGroups();

#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator <<(QDataStream &output, const CUserInfoRoster &roster);
    friend QDataStream & operator >>(QDataStream &input, CUserInfoRoster &roster);
#endif

    virtual int UpdateUserInfo(const QXmppRosterIq::Item &rosterItem);
    virtual int UpdateUserInfo(const QXmppVCardIq &vCard, QString jid);

    virtual QXmppPresence::AvailableStatusType GetStatus();//得到本地用户状态  
    virtual int SetStatus(const QString &jid, QXmppPresence::AvailableStatusType status);

    virtual QXmppRosterIq::Item::SubscriptionType GetSubScriptionType();
    QString GetSubscriptionTypeStr(QXmppRosterIq::Item::SubscriptionType type) const;
    
    int GetUnReadMessageCount();

private:
    QSet<QString> m_Groups;
    QXmppRosterIq::Item::SubscriptionType m_subscriptionType;
    QXmppPresence::AvailableStatusType m_LocalStatus;//本地用户的状态  
    int m_UnReadMessageCount;
};

#endif // USERINFOROSTER_H
