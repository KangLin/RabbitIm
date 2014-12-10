#include "User.h"
#include "UserInfoXmpp.h"
#include "Global/Global.h"

CUser::CUser(QObject *parent) :
    QObject(parent),
    m_Message(new CManageMessage)
{
#ifdef QXMPP
    QSharedPointer<CUserInfo> Info = QSharedPointer<CUserInfo>(new CUserInfoXmpp);
#else
    QSharedPointer<CUserInfo> Info = QSharedPointer<CUserInfo>(new CUserInfo);
#endif
    m_Info = Info;
}

CUser::~CUser()
{
    LOG_MODEL_DEBUG("CUser", "CUser::~CUser");
}

QSharedPointer<CUserInfo> CUser::GetInfo()
{
    return m_Info;
}

QSharedPointer<CManageMessage> CUser::GetMessage()
{
    return m_Message;
}
