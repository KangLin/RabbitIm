#ifndef GLOBALUSERQXMPP_H
#define GLOBALUSERQXMPP_H

#include "GlobalUser.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppRosterIq.h"

class CClientXmpp;
class CGlobalUserQXmpp : public CGlobalUser
{
    Q_OBJECT

public:
    CGlobalUserQXmpp(QObject *parent = 0);

    //更新本地用户信息  
    int UpdateUserInfoLocale(const QXmppVCardIq &vCard, QString jid);
    int UpdateUserInfoRoster(const QXmppRosterIq::Item &rosterItem);
    int UpdateUserInfoRoster(const QXmppVCardIq &vCard, QString jid);

private:
    //得到保存本地用户信息的文件  
    virtual QString GetLocaleFile(const QString &szLocaleJid);
    //得到保存好友信息的文件  
    virtual QString GetRosterFile(const QString &szLocaleJid);
    //新建立一个空的本地对象  
    virtual QSharedPointer<CUserInfo> NewUserInfo();

    QSharedPointer<CClientXmpp> m_Client;
};

#endif // GLOBALUSERQXMPP_H
