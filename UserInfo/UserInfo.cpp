#include "UserInfo.h"
#include "Global/Global.h"
#include <QSettings>
#include "qxmpp/QXmppUtils.h"

CUserInfo::CUserInfo(QObject *parent) :
    QObject(parent)
{
}

QString CUserInfo::GetJid()
{
    return m_szJid;
}

QString CUserInfo::GetBareJid()
{
    return QXmppUtils::jidToBareJid(GetJid());
}

QString CUserInfo::GetShowName()
{
    QString szText;
    switch(CGlobal::Instance()->GetRosterShowType())
    {
    case CGlobal::E_ROSTER_SHOW_JID:
        szText = GetBareJid();
        break;
    case CGlobal::E_ROSTER_SHOW_NAME:
        szText = GetName();
        break;
    case CGlobal::E_ROSTER_SHOW_NICK:
    default:
        szText = GetNick();
        break;
    }
    return szText;
}

QString CUserInfo::GetName()
{
    if(m_szFullName.isEmpty())
        return QXmppUtils::jidToUser(GetJid());
    return m_szFullName;
}

QString CUserInfo::GetDomain()
{
    return QXmppUtils::jidToDomain(GetJid());
}

QString CUserInfo::GetResource()
{
    return QXmppUtils::jidToResource(GetJid());
}

QString CUserInfo::GetNick()
{
    return m_szNick;
}

QDate CUserInfo::GetBirthday()
{
    return m_Birthday;
}

QString CUserInfo::GetEmail()
{
    return m_szEmail;
}

QString CUserInfo::GetDescription()
{
    return m_szDescription;
}

QImage CUserInfo::GetPhoto()
{
    return m_imgPhoto;
}

QXmppPresence::AvailableStatusType CUserInfo::GetStatus()
{
    return m_LocalStatus;
}

int CUserInfo::SetStatus(QXmppPresence::AvailableStatusType status)
{
    m_LocalStatus = status;
    return 0;
}

QDataStream & operator <<(QDataStream &output, const CUserInfo &roster)
{
    output << roster.m_szJid 
                   << roster.m_szNick 
                   << roster.m_szFullName 
                   << roster.m_szEmail 
                   << roster.m_szDescription
                   << roster.m_Birthday;
    return output;
                 
}

QDataStream & operator >>(QDataStream &input, CUserInfo &roster)
{
    input >> roster.m_szJid
               >> roster.m_szNick
               >> roster.m_szFullName
               >> roster.m_szEmail
               >> roster.m_szDescription
               >> roster.m_Birthday;
    return input;
}
