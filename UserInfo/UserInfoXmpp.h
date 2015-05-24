#ifndef USERINFOXMPP_H
#define USERINFOXMPP_H

#include <QObject>
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppRosterIq.h"
#include "UserInfo.h"

/**
 * @defgroup RABBITIM_IMPLEMENT_QXMPP_USERINFO 用户信息接口实现类模块  
 * @ingroup RABBITIM_IMPLEMENT_QXMPP_USER
 * @brief 用户信息接口类模块，用于描述用户信息  
 */

/**
 * @brief 用户信息接口qxmpp实现类，用于描述用户信息.
 *               不同协议的用户信息由具体协议派生此类实现,
 *               使用者调用 CManager::NewUserInfo() 接口得到.
 * @ingroup RABBITIM_IMPLEMENT_QXMPP_USERINFO RABBITIM_IMPLEMENT_QXMPP
 */
class CUserInfoXmpp : public CUserInfo
{
    Q_OBJECT
public:
    explicit CUserInfoXmpp(QObject *parent = 0);

    virtual QString GetShowName();
    virtual QString GetId();///< 就是 bareJid  
    virtual int SetId(QString szId);

    virtual QString GetName();
    int SetName(const QString &szName);
    virtual QString GetJid();
    virtual QString GetDomain();
    virtual QString GetResource();

    virtual int UpdateUserInfo(const QXmppVCardIq &vCard, QString jid);
    virtual int UpdateUserInfo(const QXmppRosterIq::Item &rosterItem);
    virtual int UpdateStatus(const USER_INFO_STATUS status, const QString jid);

#ifndef QT_NO_DATASTREAM
    virtual int LoadFromStorage(QDataStream &input);
    virtual int SaveToStorage(QDataStream &output);
#endif

private:
    SUBSCRIPTION_TYPE FromQxmppSubscriptionType(QXmppRosterIq::Item::SubscriptionType type);
signals:

public slots:
protected:
    QString m_szJid;
    QString m_szName;
};

#endif // USERINFOXMPP_H
