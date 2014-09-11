#include "UserInfo.h"
#include "Global/Global.h"
#include <QSettings>
#include <QBuffer>
#include <QImageReader>
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
    if(m_szName.isEmpty())
        return QXmppUtils::jidToUser(GetJid());
    return m_szName;
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
    if(m_imgPhoto.isNull())
        return QImage(":/icon/AppIcon");

    return m_imgPhoto;
}

int CUserInfo::UpdateUserInfo(const QXmppVCardIq &vCard, QString jid)
{
    if(!vCard.fullName().isEmpty())
        m_szName = vCard.fullName();
    m_szNick = vCard.nickName();
    m_Birthday = vCard.birthday();
    m_szEmail = vCard.email();
    m_szDescription = vCard.description();
    if(!jid.isEmpty())
        m_szJid = jid;

    //保存头像  
    QByteArray photo = vCard.photo();
    QBuffer buffer;
    buffer.setData(photo);
    buffer.open(QIODevice::ReadOnly);
    QImageReader imageReader(&buffer);
    m_imgPhoto = imageReader.read();
    buffer.close();

    return 0;
}

QDataStream & operator <<(QDataStream &output, const CUserInfo &roster)
{
    output << roster.m_szJid 
                   << roster.m_szNick 
                   << roster.m_szName 
                   << roster.m_szEmail 
                   << roster.m_szDescription
                   << roster.m_Birthday;
    return output;
                 
}

QDataStream & operator >>(QDataStream &input, CUserInfo &roster)
{
    input >> roster.m_szJid
               >> roster.m_szNick
               >> roster.m_szName
               >> roster.m_szEmail
               >> roster.m_szDescription
               >> roster.m_Birthday;
    return input;
}
