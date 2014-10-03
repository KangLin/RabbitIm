#include "UserInfoLocale.h"
#include <QSettings>
#include "Global.h"

CUserInfoLocale::CUserInfoLocale(QObject *parent) : CUserInfo(parent)
{
}

QXmppPresence::AvailableStatusType CUserInfoLocale::GetStatus()
{
    return m_LocalStatus;
}

int CUserInfoLocale::SetStatus(QXmppPresence::AvailableStatusType status)
{
    m_LocalStatus = status;
    QSettings conf(CGlobal::Instance()->GetUserConfigureFile(this->GetBareJid()), QSettings::IniFormat);
    conf.setValue("Login/LoginState", status);
    return 0;
}
