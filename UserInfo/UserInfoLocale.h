#ifndef USERINFOLOCALE_H
#define USERINFOLOCALE_H

#include "UserInfo.h"
#include "qxmpp/QXmppVCardIq.h"

class CUserInfoLocale : public CUserInfo
{
    Q_OBJECT
public:
    CUserInfoLocale(QObject *parent = NULL);

    virtual QXmppPresence::AvailableStatusType GetStatus();//得到本地用户状态  
    virtual int SetStatus(QXmppPresence::AvailableStatusType status);

    virtual int UpdateUserInfo(const QXmppVCardIq &vCard, QString jid);

#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator <<(QDataStream &output, const CUserInfoLocale &roster);
    friend QDataStream & operator >>(QDataStream &input, CUserInfoLocale &roster);
#endif

private:
    QXmppPresence::AvailableStatusType m_LocalStatus;//本地用户的状态  
};

#endif // USERINFOLOCALE_H
