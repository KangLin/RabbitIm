#ifndef XMPPCLIENT_H
#define XMPPCLIENT_H

#include <QObject>
#include "qxmpp/QXmppClient.h"
#include "qxmpp/QXmppLogger.h"
#include "qxmpp/QXmppTransferManager.h"
#include "qxmpp/QXmppMucManager.h"
#include "qxmpp/QXmppVCardIq.h"
#include "Client/Client.h"

class CManageUserQXmpp;

/**
 * @brief xmpp 协议客户端通信实现类  
 */
class CClientXmpp : public CClient
{
    Q_OBJECT

public:
    explicit CClientXmpp(QObject *parent = 0);
    virtual ~CClientXmpp();

    /**
     * @brief 注册新用户  
     * @param szId:用户id  
     * @param szName:用户名  
     * @param szPassword:密码  
     * @param szEmail:email  
     * @param szDescript:描述  
     * @return int：成功返回0，失败返回非0  
     * @see Login()  
     */
    virtual int Register(const QString &szId,
                         const QString &szName,
                         const QString &szPassword, 
                         const QString& szEmail = QString(),
                         const QString& szDescript = QString());
    virtual int Register(QSharedPointer<CUserInfo> userInfo,
                         const QString &szPassword);

    /**
     * @brief 登录，成功登录后会触发信号 sigClientConnected，  
     *        错误会触发信号 sigClientError  
     * @param szUserName：用户名  
     * @param szPassword：登录密码  
     * @param status：登录状态  
     * @return int：成功返回0，失败返回非0  
     * @see sigClientConnected
     * @see sigClientError
     */
    virtual int Login(const QString& szUserName = QString(), 
                      const QString &szPassword = QString(),
                      CUserInfo::USER_INFO_STATUS status = CUserInfo::Online);
    /**
     * @brief 登出,成功会触发信号 sigClientDisconnected  
     *        错误会触发信号 sigClientError  
     * @return int：成功返回0，失败返回非0  
     * @see sigClientDisconnected
     */
    virtual int Logout();

    /**
     * @brief 请求本地用户信息，会触发信号 sigUpdateLocaleUserInfo  
     *
     * @return int：成功返回0，失败返回非0  
     * @see sigUpdateLocaleUserInfo  
     */
    virtual int RequestUserInfoLocale();
    /**
     * @brief 请求指定好友的信息，会触发信号 sigUpdateRosterUserInfo  
     *
     * @param szId：好友ID
     * @return int：成功返回0，失败返回非0  
     * @see sigUpdateRosterUserInfo  
     */
    virtual int RequestUserInfoRoster(const QString& szId);
    /**
     * @brief 设置用户状态，会触发信号 sigChangedStatus· 
     *
     * @param status：状态  
     * @return int：成功返回0，失败返回非0  
     * @see sigChangedStatus
     */
    virtual int setClientStatus(CUserInfo::USER_INFO_STATUS status);

    /**
     * @brief 增加好友，会触发 sigRosterAddReceived  
     *
     * @param szId：好友id
     * @param type：订阅类型 @see SUBSCRIBE_TYPE
     * @param szName：好友名称  
     * @param groups：好友所在的组  
     * @return int：成功返回0，失败返回非0；-1:好友已经存在  
     * @see sigRosterAddReceived
     */
    virtual int RosterAdd(const QString& szId, 
                          SUBSCRIBE_TYPE type = SUBSCRIBE_REQUEST,
                          const QString &szName = QString(),
                          const QSet<QString> &groups = QSet<QString>());
    /**
     * @brief 删除好友，会触发信号 sigRosterRemoveReceived   
     *
     * @param szId：好友Id
     * @return int：成功返回0，失败返回非0  
     * @see sigRosterRemoveReceived
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
     * @brief 向好友发送消息，会触发信号 sigMessageUpdate  
     *
     * @param szId:好友id  
     * @param szMsg:消息内容  
     * @return int  
     * @see sigMessageUpdate
     */
    virtual int SendMessage(const QString& szId, const QString &szMsg);

    /**
     * @brief 设置登录用户信息  
     *
     * @param userInfo
     * @return int：成功返回0，失败返回非0  
     */
    virtual int setlocaleUserInfo(QSharedPointer<CUserInfo> userInfo);

    int SetUser(QSharedPointer<CManageUserQXmpp> user);

    /**
     * @brief 传送文件  
     *
     * @param szId：用户id  
     * @param szFile：要传送的文件 
     * @param szDescription：描述信息  
     * @return QSharedPointer<CFileTransfer>：返回 CFileTransfer 对象指针  
     * @see sigFileReceived
     */
    virtual QSharedPointer<CFileTransfer> SendFile(const QString szId, const QString &szFile, const QString &szDescription);

private:
    QXmppPresence::AvailableStatusType StatusToPresence(CUserInfo::USER_INFO_STATUS status);
    CUserInfo::USER_INFO_STATUS StatusFromPresence(QXmppPresence::AvailableStatusType status);

    int InitConnect();
    int ClearConnect();

private slots:
    void slotClientConnected();
    //void slotClientDisConnected();
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
    //接收消息  
    void slotMessageReceived(const QXmppMessage &message);

     /**
      * @brief 文件接收消息  
      *
      * @param job
      */
     void slotFileReceived(QXmppTransferJob *job);
     //qxmpp log
     void slotMessage(QXmppLogger::MessageType type, const QString &text);
     
private:
    QXmppClient m_Client;
    QSharedPointer<CManageUserQXmpp> m_User;
    QXmppMucManager m_MucManager;
    QXmppTransferManager m_TransferManager;
    QSharedPointer<CUserInfo> m_RegisterUserInfo;//用户注册信息  
    
#ifdef RABBITIM_USE_WEBRTC
    friend class CManageCallWebrtcXmpp;
#else
    friend class CManageCallXmpp;
#endif
    friend class CManageGroupChatQxmpp;
    friend class CGroupChatQxmpp;
};

#endif // XMPPCLIENT_H
