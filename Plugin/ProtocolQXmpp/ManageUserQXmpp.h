#ifndef GLOBALUSERQXMPP_H
#define GLOBALUSERQXMPP_H

#include "Manage/ManageUser.h"
#include "qxmpp/QXmppVCardIq.h"
#include "qxmpp/QXmppRosterIq.h"

/**
 * @defgroup RABBITIM_IMPLEMENT_QXMPP_MANAGEUSER 用户管理接口类模块  
 * @ingroup RABBITIM_IMPLEMENT_QXMPP_MANAGER
 * @brief 用户管理类接口类模块。
 */

class CClientXmpp;
/**
 * @class CManageUser
 * @ingroup RABBITIM_IMPLEMENT_QXMPP_MANAGEUSER RABBITIM_IMPLEMENT_QXMPP
 * @brief 用户管理类接口xmpp实现类。此类管理用户信息 (CUser),包括登录用户与好友。\n  
 *          由接口 CManager::GetManageUser() 访问此接口对象。\n
 *          登录成功后调用 Init\n  
 *          GetUserInfoLocale 需要访问登录用户信息\n  
 *          GetUserInfoRoster 需要访问好友用户信息\n  
 *          登出后调用 Clean  
 * @see CManager 
 * @see CUser  
 */
class CManageUserQXmpp : public CManageUser
{
    Q_OBJECT

public:
    CManageUserQXmpp(QObject *parent = 0);

    /**
     * @brief 新增加一个空的好友对象，并把此好友对象插入到好友列表中  
     *
     * @param szId：新增加好的ID  
     * @return QSharedPointer<CUserInfo>:成功,返回好友对象.失败,返回空  
     */
    virtual QSharedPointer<CUser> AddUserInfoRoster(const QString &szId);

    //更新本地用户信息  
    int UpdateUserInfoLocale(const QXmppVCardIq &vCard, QString jid);
    int UpdateUserInfoRoster(const QXmppRosterIq::Item &rosterItem);
    int UpdateUserInfoRoster(const QXmppVCardIq &vCard, QString jid);

    //新建立一个空的本地对象  
    virtual QSharedPointer<CUser> NewUser();
private:
    //得到保存本地用户信息的文件  
    virtual QString GetLocaleFile(const QString &szLocaleJid);
    //得到保存好友信息的文件  
    virtual QString GetRosterFile(const QString &szLocaleJid);
};

#endif // GLOBALUSERQXMPP_H
