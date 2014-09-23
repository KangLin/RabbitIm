#ifndef XMPPCLIENT_H
#define XMPPCLIENT_H

#include <QObject>
#include "qxmpp/QXmppClient.h"
#include "qxmpp/QXmppLogger.h"
#include "qxmpp/QXmppCallManager.h"
#include "qxmpp/QXmppTransferManager.h"
#include "qxmpp/QXmppMucManager.h"
#include "qxmpp/QXmppVCardIq.h"
#include "Client.h"

class CGlobalUserQXmpp;
class CClientXmpp : public CClient
{
    Q_OBJECT

public:
    explicit CClientXmpp(QObject *parent = 0);
    virtual ~CClientXmpp();

    /**
     * @brief 注册新用户  
     *
     * @param szId:用户id  
     * @param szName:用户名  
     * @param szPassword:密码  
     * @param szEmail:email  
     * @param szDescript:描述  
     * @return int
     */
    virtual int Register(const QString &szId,
                         const QString &szName,
                         const QString &szPassword, 
                         const QString& szEmail = QString(),
                         const QString& szDescript = QString());
    /**
     * @brief 登录  
     *
     * @param szUserName：用户名  
     * @param szPassword：登录密码  
     * @param status：登录状态  
     * @return int：成功返回0，失败返回非0  
     */
    virtual int Login(const QString& szUserName = QString(), 
                      const QString &szPassword = QString(),
                      CUserInfo::USER_INFO_STATUS status = CUserInfo::Online);
    virtual int Logout();
    //请求本地用户信息  
    virtual int RequestUserInfoLocale();
    //请求指定好友的信息  
    virtual int RequestUserInfoRoster(const QString& szId);
    //设置用户状态· 
    virtual int setClientStatus(CUserInfo::USER_INFO_STATUS status);

    /**
     * @brief //增加好友  
     *
     * @param szId：好友id
     * @return int：成功返回0，失败返回非0  
     */
    virtual int RosterAdd(const QString& szId, SUBSCRIBE_TYPE type = SUBSCRIBE_REQUEST,
                          const QString &szName = QString(), const QSet<QString> &groups = QSet<QString>());
    /**
     * @brief 删除好友  
     *
     * @param szId：好友Id
     * @return int：成功返回0，失败返回非0  
     */
    virtual int RosterRemove(const QString& szId);
    /**
     * @brief 重命名好友名称  
     *
     * @param szId:好友id  
     * @param szName:好友名称  
     * @return int：成功返回0，失败返回非0  
     */
    virtual int RosterRename(const QString& szId, const QString& szName);

    /**
     * @brief 向好友发送消息  
     *
     * @param szId:好友id  
     * @param szMsg:消息内容  
     * @return int
     */
    virtual int SendMessage(const QString& szId, const QString &szMsg);

    /**
     * @brief 设置登录用户信息  
     *
     * @param userInfo
     * @return int
     */
    virtual int setlocaleUserInfo(QSharedPointer<CUserInfo> userInfo);

    int SetUser(QSharedPointer<CGlobalUserQXmpp> user);

private:
    QXmppPresence::AvailableStatusType StatusToPresence(CUserInfo::USER_INFO_STATUS status);
    CUserInfo::USER_INFO_STATUS StatusFromPresence(QXmppPresence::AvailableStatusType status);

    int InitConnect();

private slots:
    void slotClientConnected();
    void slotClientDisConnected();
    void slotClientError(QXmppClient::Error e);
    void slotClientIqReceived(const QXmppIq &iq);
    void slotStateChanged(QXmppClient::State state);
    void slotSubscriptionReceived(const QString& szId);
    void slotRosterReceived();
    void slotClientVCardReceived();
    //得到好友详细信息时触发  
    void slotvCardReceived(const QXmppVCardIq&vCardIq);
    void slotPresenceReceived(const QXmppPresence &presence);
    void slotItemAdded(const QString &szId);
    void slotItemChanged(const QString& szId);
    void slotItemRemoved(const QString& szId);

private:
    QXmppClient m_Client;
    QSharedPointer<CGlobalUserQXmpp> m_User;
    QXmppCallManager m_CallManager;
    QXmppMucManager m_MucManager;
    QXmppTransferManager m_TransferManager;
};

#endif // XMPPCLIENT_H
