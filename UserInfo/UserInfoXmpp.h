#ifndef USERINFOXMPP_H
#define USERINFOXMPP_H

#include <QObject>
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppRosterIq.h"
#include "UserInfo.h"

class CUserInfoXmpp : public CUserInfo
{
    Q_OBJECT
public:
    explicit CUserInfoXmpp(QObject *parent = 0);

    virtual QString GetShowName();
    virtual QString GetId();//就是 bareJid  
    virtual int SetId(QString szId);

    virtual QString GetName();
    virtual QString GetJid();
    virtual QString GetDomain();
    virtual QString GetResource();

    virtual int UpdateUserInfo(const QXmppVCardIq &vCard, QString jid);
    virtual int UpdateUserInfo(const QXmppRosterIq::Item &rosterItem);

#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator <<(QDataStream &output, const CUserInfoXmpp &roster);
    friend QDataStream & operator >>(QDataStream &input, CUserInfoXmpp &roster);
#endif

private:
    SUBSCRIPTION_TYPE FromQxmppSubscriptionType(QXmppRosterIq::Item::SubscriptionType type);
signals:

public slots:
protected:
    QString m_szJid;
    QString m_szName;
};

#endif // USERINFOXMPP_H
