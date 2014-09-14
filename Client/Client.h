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

public slots:

};

#endif // CLIENT_H
