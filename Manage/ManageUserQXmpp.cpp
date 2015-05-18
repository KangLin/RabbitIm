#include "ManageUserQXmpp.h"
#include "Global/Global.h"
#include "UserInfo/UserInfoXmpp.h"
#include "Client/ClientXmpp.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppVCardManager.h"
#include "qxmpp/QXmppUtils.h"
#include <QDir>

CManageUserQXmpp::CManageUserQXmpp(QObject *parent) 
    : CManageUser(parent)
{
}

QSharedPointer<CUser> CManageUserQXmpp::NewUser()
{
    QSharedPointer<CUser> user(new CUser);
    return user;
}

/**
 * @brief 新增加一个空的好友对象，并把此好友对象插入到好友列表中  
 *
 * @param szId：新增加好的ID  
 * @return QSharedPointer<CUserInfo>:成功,返回好友对象.失败,返回空  
 */
QSharedPointer<CUser> CManageUserQXmpp::AddUserInfoRoster(const QString &szId)
{
    QSharedPointer<CUser> user =this->GetUserInfoRoster(szId);
    if(!user.isNull())
    {
        LOG_MODEL_ERROR("CGlobalUserQXmpp", "AddUserInfoRoster:roster is exist:%s", szId.toStdString().c_str());
        return user;
    }

    SetModify(true);
    user = NewUser();
    ((CUserInfoXmpp*)user->GetInfo().data())->SetId(szId);
    m_UseRoster.insert(szId, user);
    return user;
}

int CManageUserQXmpp::UpdateUserInfoLocale(const QXmppVCardIq &vCard, QString jid)
{
    SetModify(true);
    if(m_UserLocale.isNull())
    {
        m_UserLocale = NewUser();
    }

    ((CUserInfoXmpp*)m_UserLocale->GetInfo().data())->UpdateUserInfo(vCard, jid);
    return 0;
}

int CManageUserQXmpp::UpdateUserInfoRoster(const QXmppRosterIq::Item &rosterItem)
{
    int nRet = 0;
    QString jid = rosterItem.bareJid();
    QSharedPointer<CUser> roster = GetUserInfoRoster(jid);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CGlobalUserQXmpp", "There are not the roster:%s", jid.toStdString().c_str());
        return -1;
    }
    SetModify(true);
    nRet = ((CUserInfoXmpp*)roster->GetInfo().data())->UpdateUserInfo(rosterItem);
    return nRet;
}

int CManageUserQXmpp::UpdateUserInfoRoster(const QXmppVCardIq &vCard, QString jid)
{
    int nRet = 0;
    QString szBareJid = QXmppUtils::jidToBareJid(jid);
    QSharedPointer<CUser> roster = GetUserInfoRoster(szBareJid);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CGlobalUserQXmpp", "There are not the roster:%s", jid.toStdString().c_str());
        return -1;
    }
    SetModify(true);
    nRet = ((CUserInfoXmpp*)roster->GetInfo().data())->UpdateUserInfo(vCard, jid);
    return nRet;
}

QString CManageUserQXmpp::GetLocaleFile(const QString &szLocaleJid)
{
    return CGlobal::Instance()->GetDirUserData(szLocaleJid) 
            + QDir::separator() + "XmppLocaleInfo.dat";
}

QString CManageUserQXmpp::GetRosterFile(const QString &szLocaleJid)
{
    return CGlobal::Instance()->GetDirUserData(szLocaleJid) 
            + QDir::separator() + "XmppRosterInfo.dat";
}
