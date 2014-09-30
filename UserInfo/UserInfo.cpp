#include "UserInfo.h"
#include "../Global/Global.h"
#include <QSettings>
#include <QBuffer>
#include <QImageReader>

CUserInfo::CUserInfo(QObject *parent) :
    QObject(parent)
{
    m_subscriptionType = None;
    m_Status = OffLine;
}

CUserInfo::~CUserInfo()
{
}

QString CUserInfo::GetShowName()
{
    LOG_MODEL_WARNING("CUserInfo", "The GetShowName function must be implemented by derived classes");
    Q_ASSERT(false);
    return QString();
}

QString CUserInfo::GetId()
{
    LOG_MODEL_WARNING("CUserInfo", "The GetId function must be implemented by derived classes");
    Q_ASSERT(false);
    return QString();
}

QString CUserInfo::GetName()
{
    LOG_MODEL_WARNING("CUserInfo", "The GetName function must be implemented by derived classes");
    Q_ASSERT(false);
    return QString();
}

int CUserInfo::SetName(const QString &szName)
{
    Q_UNUSED(szName);
    LOG_MODEL_WARNING("CUserInfo", "The SetName function must be implemented by derived classes");
    Q_ASSERT(false);
    return 0;
}

QString CUserInfo::GetNick()
{
    return m_szNick;
}

int CUserInfo::SetNick(const QString &szNick)
{
    m_szNick = szNick;
    return 0;
}

QDate CUserInfo::GetBirthday()
{
    return m_Birthday;
}

int CUserInfo::SetBirthday(const QDate &d)
{
    m_Birthday = d;
    return 0;
}

QString CUserInfo::GetEmail()
{
    return m_szEmail;
}

int CUserInfo::SetEmail(const QString &szEmail)
{
    m_szEmail = szEmail;
    return 0;
}

QString CUserInfo::GetDescription()
{
    return m_szDescription;
}

int CUserInfo::SetDescription(const QString &szDescription)
{
    m_szDescription = szDescription;
    return 0;
}

QImage CUserInfo::GetPhoto()
{
    if(m_imgPhoto.isNull())
        return QImage(":/icon/AppIcon");

    return m_imgPhoto;
}

int CUserInfo::SetPhoto(QByteArray img)
{
    QBuffer buf(&img);
    QImageReader reader(&buf, "png");
    m_imgPhoto = reader.read();
    buf.close();
    return 0;
}

int CUserInfo::SetPhoto(QImage img)
{
    m_imgPhoto = img;
    return 0;
}

int CUserInfo::SetStatus(USER_INFO_STATUS status)
{
    m_Status = status;
    return 0;
}

CUserInfo::USER_INFO_STATUS CUserInfo::GetStatus()
{
    return m_Status;
}

QSet<QString> &CUserInfo::GetGroups()
{
    return m_Groups;
}

CUserInfo::SUBSCRIPTION_TYPE CUserInfo::GetSubScriptionType()
{
    return m_subscriptionType;
}

QString CUserInfo::GetSubscriptionTypeStr(SUBSCRIPTION_TYPE type) const
{
    switch(type)
    {
    case NotSet:
        return "";
    case None:
        return tr("[none]");
    case Both:
        return "";
    case From:
        return tr("[From]");
    case To:
        return "";
    case Remove:
        return tr("[remove]");
    default:
        {
            LOG_MODEL_WARNING("Roster", "QXmppRosterIq::Item::getTypeStr(): invalid type");
            return "";
        }
    }
}

int CUserInfo::LoadFromStorage(QDataStream &input)
{
    int nRet = 0;
    input >> m_szNick
          >> m_szEmail
          >> m_szDescription
          >> m_Birthday
          >> (int&)m_subscriptionType;
    int nSize = 0;
    input >> nSize;
    while (nSize--) {
        QString group;
        input >> group;
        m_Groups << group;
    }
    return nRet;
}

int CUserInfo::SaveToStorage(QDataStream &output)
{
    int nRet = 0;
    output << m_szNick 
           << m_szEmail 
           << m_szDescription
           << m_Birthday
           << (int&)m_subscriptionType
           << m_Groups.size();
    foreach(QString group, m_Groups)
    {
        output << group;
    }
    return nRet;
}
