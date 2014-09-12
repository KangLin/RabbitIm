#include "UserInfoRoster.h"
#include "../Global/Global.h"
#include <QSettings>
#include <QBuffer>
#include <QImageReader>
#include <QImageWriter>

CUserInfoRoster::CUserInfoRoster(QObject *parent) : CUserInfo(parent)
{
    m_UnReadMessageCount = 0;
}

QSet<QString> &CUserInfoRoster::GetGroups()
{
    return m_Groups;
}

int CUserInfoRoster::UpdateUserInfo(const QXmppRosterIq::Item &rosterItem)
{
    m_szJid = rosterItem.bareJid();
    m_Groups = rosterItem.groups();
    return 0;
}

int CUserInfoRoster::UpdateUserInfo(const QXmppVCardIq &vCard, QString jid)
{
    int nRet = 0;
    nRet = CUserInfo::UpdateUserInfo(vCard, jid);
    if(nRet)
        return nRet;

    //保存头像到本地  
    QImageWriter imageWriter(CGlobal::Instance()->GetFileUserAvatar(GetBareJid()), "png");
    if(!imageWriter.write(GetPhoto()))
        LOG_MODEL_ERROR("CUserInfo", "Save avater error, %s", imageWriter.errorString().toStdString().c_str());
    return 0;
}

QXmppPresence::AvailableStatusType CUserInfoRoster::GetStatus()
{
    return m_LocalStatus;
}

int CUserInfoRoster::SetStatus(const QString &jid, QXmppPresence::AvailableStatusType status)
{
    if(jid.isEmpty())
    {
        LOG_MODEL_ERROR("UserInfo", "jid is null");
        Q_ASSERT(false);
    }
    m_szJid = jid;
    m_LocalStatus = status;
    return 0;
}

QXmppRosterIq::Item::SubscriptionType CUserInfoRoster::GetSubScriptionType()
{
    return m_subscriptionType;
}

QString CUserInfoRoster::GetSubscriptionTypeStr(QXmppRosterIq::Item::SubscriptionType type) const
{
    switch(type)
    {
    case QXmppRosterIq::Item::NotSet:
        return "";
    case QXmppRosterIq::Item::None:
        return tr("[none]");
    case QXmppRosterIq::Item::Both:
        return "";
    case QXmppRosterIq::Item::From:
        return tr("[From]");
    case QXmppRosterIq::Item::To:
        return "";
    case QXmppRosterIq::Item::Remove:
        return tr("[remove]");
    default:
        {
            LOG_MODEL_WARNING("Roster", "QXmppRosterIq::Item::getTypeStr(): invalid type");
            return "";
        }
    }
}

int CUserInfoRoster::GetUnReadMessageCount()
{
    return m_UnReadMessageCount;
}

QDataStream & operator <<(QDataStream &output, const CUserInfoRoster &roster)
{
    output << (CUserInfo&)roster;
    output << roster.m_Groups.size();
    QString szGroup;
    foreach(szGroup, roster.m_Groups)
    {
        output << szGroup;
    }
    return output;
}

QDataStream & operator >>(QDataStream &input, CUserInfoRoster &roster)
{
    input >> (CUserInfo&)roster;
    int nSize = 0;
    input >> nSize;
    while(nSize--)
    {
        QString szGroup;
        input >> szGroup;
        roster.m_Groups << szGroup;
    }
    return input;
}
