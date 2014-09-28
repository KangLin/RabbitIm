#include "User.h"
#include "UserInfoXmpp.h"

CUser::CUser(QObject *parent) :
    QObject(parent)
{
#ifdef QXMPP
    QSharedPointer<CUserInfo> Info = QSharedPointer<CUserInfo>(new CUserInfoXmpp);
#else
    QSharedPointer<CUserInfo> Info = QSharedPointer<CUserInfo>(new CUserInfo);
#endif
    m_Info = Info;
}

QSharedPointer<CUserInfo> CUser::GetInfo()
{
    return m_Info;
}

QSharedPointer<CMessage> CUser::GetMessage()
{
    return m_Message;
}
