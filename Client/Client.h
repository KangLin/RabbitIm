#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "../UserInfo/UserInfo.h"

/**
 * @brief 通信客户端抽象类  
 *
 */
class CClient : public QObject
{
    Q_OBJECT
public:
    explicit CClient(QObject *parent = 0);
    virtual ~CClient();

    /**
     * @brief 登录  
     *
     * @param szUserName：用户名  
     * @param szPassword：登录密码  
     * @param status：登录状态  
     * @return int：成功返回0，失败返回非0  
     */
    virtual int Login(const QString& szUserName, 
                      const QString &szPassword,
                      CUserInfo::USER_INFO_STATUS status = CUserInfo::Online);
    virtual int Logout();
    /**
     * @brief 请求本地用户信息  
     *
     * @return int：成功返回0，失败返回非0  
     */
    virtual int RequestUserInfoLocale();
    /**
     * @brief 请求指定好友的信息  
     *
     * @param szId：好友ID
     * @return int：成功返回0，失败返回非0  
     */
    virtual int RequestUserInfoRoster(const QString& szId);
    /**
     * @brief 设置用户状态· 
     *
     * @param status：状态  
     * @return int：成功返回0，失败返回非0  
     */
    virtual int setClientStatus(CUserInfo::USER_INFO_STATUS status);

    /**
     * @brief //增加好友  
     *
     * @param szId：好友id
     * @return int：成功返回0，失败返回非0  
     */
    virtual int RosterSubscribe(const QString& szId, const QString &szName = QString(), const QSet<QString> &groups = QSet<QString>());
    /**
     * @brief 删除好友  
     *
     * @param szId：好友Id
     * @return int：成功返回0，失败返回非0  
     */
    virtual int RosterUnsubscribe(const QString& szId);

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
    /**
     * @brief 新增好友  
     *
     * @param szId：好友ID  
     */
    void sigInsertRoster(const QString& szId);
    /**
     * @brief 更新好友信息  
     *
     * @param szId：更新好友信息  
     */
    void sigUpdateRosterUserInfo(const QString &szId);
    /**
     * @brief 更新本地用户信息  
     *
     */
    void sigUpdateLocaleUserInfo();

    /**
     * @brief 接收增加好友请求时触发  
     *
     * @param szId：请求者ID  
     */
    void sigRosterSubscriptionReceived(const QString& szId);
    /**
     * @brief 删除好友请求时触发  
     *
     * @param szId：删除好友的ID  
     */
    void sigRosterUnsubscriptionReceived(const QString& szId);

public slots:

};

#endif // CLIENT_H
