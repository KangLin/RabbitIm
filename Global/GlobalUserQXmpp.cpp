#include "GlobalUserQXmpp.h"
#include "Global.h"
#include "UserInfo/UserInfoXmpp.h"
#include "Client/ClientXmpp.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppVCardManager.h"
#include "qxmpp/QXmppUtils.h"
#include <QDir>

CGlobalUserQXmpp::CGlobalUserQXmpp(QObject *parent) 
    : CGlobalUser(parent)
{
}

QSharedPointer<CUserInfo> CGlobalUserQXmpp::NewUserInfo()
{
    QSharedPointer<CUserInfo> user(new CUserInfoXmpp);
    return user;
}

int CGlobalUserQXmpp::UpdateUserInfoLocale(const QXmppVCardIq &vCard, QString jid)
{
    m_bModify = true;
    if(m_UserInforLocale.isNull())
    {
        m_UserInforLocale = NewUserInfo();
    }

    ((CUserInfoXmpp*)m_UserInforLocale.data())->UpdateUserInfo(vCard, jid);
    return 0;
}

int CGlobalUserQXmpp::UpdateUserInfoRoster(const QXmppRosterIq::Item &rosterItem)
{
    int nRet = 0;
    m_bModify = true;
    QString jid = rosterItem.bareJid();
    QSharedPointer<CUserInfo> roster = GetUserInfoRoster(jid);
    if(roster.isNull())
    {
        roster = NewUserInfo();
        m_UserInfoRoster.insert(jid, roster);
    }
    nRet = ((CUserInfoXmpp*)roster.data())->UpdateUserInfo(rosterItem);
    return nRet;
}

int CGlobalUserQXmpp::UpdateUserInfoRoster(const QXmppVCardIq &vCard, QString jid)
{
    int nRet = 0;
    m_bModify = true;
    QString szBareJid = QXmppUtils::jidToBareJid(jid);
    QSharedPointer<CUserInfo> roster = GetUserInfoRoster(szBareJid);
    if(roster.isNull())
    {
        roster = NewUserInfo();
        m_UserInfoRoster.insert(szBareJid, roster);
    }
    nRet = ((CUserInfoXmpp*)roster.data())->UpdateUserInfo(vCard, jid);
    return nRet;
}

QString CGlobalUserQXmpp::GetLocaleFile(const QString &szLocaleJid)
{
    return CGlobal::Instance()->GetDirUserData(szLocaleJid) 
            + QDir::separator() + "XmppLocaleInfo.dat";
}

QString CGlobalUserQXmpp::GetRosterFile(const QString &szLocaleJid)
{
    return CGlobal::Instance()->GetDirUserData(szLocaleJid) 
            + QDir::separator() + "XmppRosterInfo.dat";
}
