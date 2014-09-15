#ifndef XMPPCLIENT_H
#define XMPPCLIENT_H

#include <QObject>
#include "qxmpp/QXmppClient.h"
#include "qxmpp/QXmppLogger.h"
#include "qxmpp/QXmppCallManager.h"
#include "qxmpp/QXmppTransferManager.h"
#include "qxmpp/QXmppMucManager.h"
#include "Client.h"

class CGlobalUserQXmpp;
class CClientXmpp : public CClient
{
    Q_OBJECT

public:
    explicit CClientXmpp(QObject *parent = 0);
    virtual ~CClientXmpp();

    //登录  
    virtual int Login(const QString& szUserName, 
                      const QString &szPassword,
                      CUserInfo::USER_INFO_STATUS status = CUserInfo::Online);
    //请求本地用户信息  
    virtual int RequestUserInfoLocale();
    //请求指定好友的信息  
    virtual int RequestUserInfoRoster(const QString& szId);
    //设置用户状态· 
    virtual int setClientStatus(CUserInfo::USER_INFO_STATUS status);

    //订阅  
    virtual int RosterSubscribe(const QString& szId);

private:
    QXmppPresence::AvailableStatusType StatusToPresence(CUserInfo::USER_INFO_STATUS status);
    CUserInfo::USER_INFO_STATUS StatusFromPresence(QXmppPresence::AvailableStatusType status);

private slots:
    void slotClientError(QXmppClient::Error e);
    void slotClientIqReceived(const QXmppIq &iq);
    void slotStateChanged(QXmppClient::State state);
    void slotRosterReceived();
    void slotClientVCardReceived();
    //得到好友详细信息时触发  
    void slotvCardReceived(const QXmppVCardIq&);
    void slotPresenceReceived(const QXmppPresence &presence);

private:
    QXmppClient m_Client;
    QSharedPointer<CGlobalUserQXmpp> m_User;
    QXmppCallManager m_CallManager;
    QXmppMucManager m_MucManager;
    QXmppTransferManager m_TransferManager;
};

#endif // XMPPCLIENT_H
