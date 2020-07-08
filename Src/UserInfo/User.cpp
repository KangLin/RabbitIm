#include "User.h"
#include "Global/Global.h"

CUser::CUser(QObject *parent) :
    QObject(parent),
    m_Message(new CManageMessage)
{
    m_Info = GETMANAGER->NewUserInfo();
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
