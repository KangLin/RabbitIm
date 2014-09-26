#include "GlobalUserQXmpp.h"
#include "Global.h"
#include "UserInfo/UserInfoXmpp.h"
#include "Client/ClientXmpp.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppVCardManager.h"
#include "qxmpp/QXmppUtils.h"
#include <QDir>

CGlobalUserQXmpp::CGlobalUserQXmpp(QObject *parent) 
    : CManageUserInfo(parent)
{
}

QSharedPointer<CUserInfo> CGlobalUserQXmpp::NewUserInfo()
{
    QSharedPointer<CUserInfo> user(new CUserInfoXmpp);
    return user;
}

/**
 * @brief 新增加一个空的好友对象，并把此好友对象插入到好友列表中  
 *
 * @param szId：新增加好的ID  
 * @return QSharedPointer<CUserInfo>:成功,返回好友对象.失败,返回空  
 */
QSharedPointer<CUserInfo> CGlobalUserQXmpp::AddUserInfoRoster(const QString &szId)
{
    QSharedPointer<CUserInfo> user =this->GetUserInfoRoster(szId);
    if(!user.isNull())
    {
        LOG_MODEL_ERROR("CGlobalUserQXmpp", "AddUserInfoRoster:roster is exist:%s", szId.toStdString().c_str());
        return user;
    }

    SetModify(true);
    user = NewUserInfo();
    ((CUserInfoXmpp*)user.data())->SetId(szId);
    m_UserInfoRoster.insert(szId, user);
    return user;
}

int CGlobalUserQXmpp::UpdateUserInfoLocale(const QXmppVCardIq &vCard, QString jid)
{
    SetModify(true);
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
    QString jid = rosterItem.bareJid();
    QSharedPointer<CUserInfo> roster = GetUserInfoRoster(jid);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CGlobalUserQXmpp", "There are not the roster:%s", jid.toStdString().c_str());
        return -1;
    }
    SetModify(true);
    nRet = ((CUserInfoXmpp*)roster.data())->UpdateUserInfo(rosterItem);
    return nRet;
}

int CGlobalUserQXmpp::UpdateUserInfoRoster(const QXmppVCardIq &vCard, QString jid)
{
    int nRet = 0;
    QString szBareJid = QXmppUtils::jidToBareJid(jid);
    QSharedPointer<CUserInfo> roster = GetUserInfoRoster(szBareJid);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CGlobalUserQXmpp", "There are not the roster:%s", jid.toStdString().c_str());
        return -1;
    }
    SetModify(true);
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
