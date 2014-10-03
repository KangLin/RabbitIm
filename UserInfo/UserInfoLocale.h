#ifndef USERINFOLOCALE_H
#define USERINFOLOCALE_H

#include "UserInfo.h"

class CUserInfoLocale : public CUserInfo
{
    Q_OBJECT
public:
    CUserInfoLocale(QObject *parent);

    virtual QXmppPresence::AvailableStatusType GetStatus();//得到本地用户状态  
    virtual int SetStatus(QXmppPresence::AvailableStatusType status);

};

#endif // USERINFOLOCALE_H
