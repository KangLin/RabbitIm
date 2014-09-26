#ifndef GLOBALUSERQXMPP_H
#define GLOBALUSERQXMPP_H

#include "ManageUserInfo.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppRosterIq.h"

class CClientXmpp;
class CManageUserInfoQXmpp : public CManageUserInfo
{
    Q_OBJECT

public:
    CManageUserInfoQXmpp(QObject *parent = 0);

    /**
     * @brief 新增加一个空的好友对象，并把此好友对象插入到好友列表中  
     *
     * @param szId：新增加好的ID  
     * @return QSharedPointer<CUserInfo>:成功,返回好友对象.失败,返回空  
     */
    virtual QSharedPointer<CUserInfo> AddUserInfoRoster(const QString &szId);

    //更新本地用户信息  
    int UpdateUserInfoLocale(const QXmppVCardIq &vCard, QString jid);
    int UpdateUserInfoRoster(const QXmppRosterIq::Item &rosterItem);
    int UpdateUserInfoRoster(const QXmppVCardIq &vCard, QString jid);

private:
    //得到保存本地用户信息的文件  
    virtual QString GetLocaleFile(const QString &szLocaleJid);
    //得到保存好友信息的文件  
    virtual QString GetRosterFile(const QString &szLocaleJid);
    //新建立一个空的本地对象  
    virtual QSharedPointer<CUserInfo> NewUserInfo();
};

#endif // GLOBALUSERQXMPP_H
