#include "UserInfoLocale.h"
#include <QSettings>
#include "Global/Global.h"

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

QDataStream & operator <<(QDataStream &output, const CUserInfoLocale &roster)
{
    /*output << roster.m_szJid 
                   << roster.m_szNick 
                   << roster.m_szFullName 
                   << roster.m_szEmail 
                   << roster.m_szDescription
                   << roster.m_Birthday;//*/
    output << (CUserInfo&)roster;
    return output;
}

QDataStream & operator >>(QDataStream &input, CUserInfoLocale &roster)
{
    /*input >> roster.m_szJid
               >> roster.m_szNick
               >> roster.m_szFullName
               >> roster.m_szEmail
               >> roster.m_szDescription
               >> roster.m_Birthday;//*/
    input >> (CUserInfo&)roster;
    return input;
}

