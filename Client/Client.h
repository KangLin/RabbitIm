#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "../UserInfo/UserInfo.h"

class CClient : public QObject
{
    Q_OBJECT
public:
    explicit CClient(QObject *parent = 0);
    virtual ~CClient();

    //登录  
    virtual int Login(const QString& szUserName, 
                      const QString &szPassword,
                      CUserInfo::USER_INFO_STATUS status = CUserInfo::Online);
    virtual int Logout();
    //请求本地用户信息  
    virtual int RequestUserInfoLocale();
    //请求指定好友的信息  
    virtual int RequestUserInfoRoster(const QString& szId);
    //设置用户状态· 
    virtual int setClientStatus(CUserInfo::USER_INFO_STATUS status);

    //订阅  
    virtual int RosterSubscribe(const QString& szId);
    virtual int RosterAdd(const QString &szId, const QString &name = QString(), const QSet<QString> &groups = QSet<QString>());

    enum ERROR_TYPE
    {
        NoError,            ///< No error.
        NetworkError,        ///< Error due to TCP socket.
        KeepAliveError,     ///< Error due to no response to a keep alive.
        LoginFail          ///< 登录失败  
    };

signals:
    void sigClientConnected();
    void sigClientDisconnected();
    void sigClientError(CClient::ERROR_TYPE e);

    //好友状态改变时触发  
    void sigChangedStatus(const QString& szId);
    //从文件中加载好友完成后触发  
    void sigLoadRosterFromStorage();
    //新增好友  
    void sigInsertRoster(const QString& szId);
    //更新好友信息  
    void sigUpdateRosterUserInfo(const QString &szId);
    //更新本地用户信息  
    void sigUpdateLocaleUserInfo();

    //接收好友订阅请求时触发  
    void sigSubscriptionReceived(const QString& szId);

public slots:

};

#endif // CLIENT_H
