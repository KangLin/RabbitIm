#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "../UserInfo/User.h"
#include "FileTransfer/FileTransfer.h"
#include "Call/CallObject.h"
#include "UserInfo/COperateRoster.h"

/**
 * @defgroup RABBITIM_CLIENT 客户端通信抽象接口类模块  
 * @ingroup RABBITIM_INTERFACE_MANAGER  
 * @brief 客户端通信抽象接口类模块。\n  
 */

/**
 * @class CClient
 * @ingroup RABBITIM_CLIENT RABBITIM_INTERFACE
 * @brief 客户端通信抽象接口类。\n  
 *        包括注册、登录、登出、好友操作、文件传输、音视频呼叫等操作。\n  
 *        这个只是一个接口。具体的功能由相关协议类实现。\n  
 *        xmpp 协议由 CClientXmpp 类实现。\n  
 *        由接口 CManager::GetClient() 访问此对象  
 * @see CManager
 * @see CUser
 * @see CUserInfo
 * @see CManageUser
 * @see CManageFileTransfer
 * @see CManageCall
 */
class RABBITIM_SHARED_LIBRARY CClient : public QObject, COperateRoster
{
    Q_OBJECT
public:
    explicit CClient(QObject *parent = 0);
    virtual ~CClient();

    virtual bool IsLogin();///< 用户是否登录  

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
                         const QString& szDescript = QString()) = 0;
    virtual int Register(QSharedPointer<CUserInfo> userInfo, 
                         const QString &szPassword) = 0;

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
                      CUserInfo::USER_INFO_STATUS status = CUserInfo::Online) = 0;
    /**
     * @brief 登出,成功会触发信号 sigClientDisconnected  
     *        错误会触发信号 sigClientError  
     * @return int：成功返回0，失败返回非0  
     * @see sigClientDisconnected
     */
    virtual int Logout() = 0;

    /**
     * @brief 请求本地用户信息，会触发信号 sigUpdateLocaleUserInfo  
     *
     * @return int：成功返回0，失败返回非0  
     * @see sigUpdateLocaleUserInfo  
     */
    virtual int RequestUserInfoLocale() = 0;
    /**
     * @brief 请求指定好友的信息，会触发信号 sigUpdateRosterUserInfo  
     *
     * @param szId：好友ID
     * @return int：成功返回0，失败返回非0  
     * @see sigUpdateRosterUserInfo  
     */
    virtual int RequestUserInfoRoster(const QString& szId) = 0;
    /**
     * @brief 刷新好友列表  
     * @return 
     */
    virtual int RefreshRosterList();
    //内部使用  
    virtual int ProcessRoster(QSharedPointer<CUserInfo> roster, void *para = NULL);
    /**
     * @brief 设置用户状态，会触发信号 sigChangedStatus· 
     *
     * @param status：状态  
     * @return int：成功返回0，失败返回非0  
     * @see sigChangedStatus
     */
    virtual int setClientStatus(CUserInfo::USER_INFO_STATUS status) = 0;

    /**
     * @brief 订阅类型  
     * A:请求订阅B\n  
     * B:接收 A 订阅请求信号\n  
     *   - 同意：  
     *   - 拒绝：  
     */
    enum SUBSCRIBE_TYPE
    {
        SUBSCRIBE_REQUEST, ///< 请求订阅  
        SUBSCRIBE_ACCEPT,  ///< 接受订阅  
        SUBSCRIBE_REFUSE   ///< 拒绝订阅  
    };
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
                          const QSet<QString> &groups = QSet<QString>()) = 0;
    /**
     * @brief 删除好友，会触发信号 sigRosterRemoveReceived   
     *
     * @param szId：好友Id
     * @return int：成功返回0，失败返回非0  
     * @see sigRosterRemoveReceived
     */
    virtual int RosterRemove(const QString& szId) = 0;
    /**
     * @brief 重命名好友名称  
     *
     * @param szId:好友id  
     * @param szName:好友名称  
     * @return int：成功返回0，失败返回非0  
     */
    virtual int RosterRename(const QString& szId, const QString& szName) = 0;

    /**
     * @brief 向好友发送消息，会触发信号 sigMessageUpdate  
     *
     * @param szId:好友id  
     * @param szMsg:消息内容  
     * @return int  
     * @see sigMessageUpdate
     */
    virtual int SendMessage(const QString& szId, const QString &szMsg) = 0;
    /**
     * @brief 设置登录用户信息  
     *
     * @param userInfo
     * @return int：成功返回0，失败返回非0  
     */
    virtual int setlocaleUserInfo(QSharedPointer<CUserInfo> userInfo) = 0;

    /**
     * @brief 传送文件  
     *
     * @param szId：用户id  
     * @param szFile：要传送的文件 
     * @param szDescription：描述信息  
     * @return QSharedPointer<CFileTransfer>：返回 CFileTransfer 对象指针  
     * @see sigFileReceived
     */
    virtual QSharedPointer<CFileTransfer> SendFile(const QString szId, 
             const QString &szFile, const QString &szDescription) = 0;

    enum ERROR_TYPE
    {
        NoError,             ///< 没有错误.  
        NetworkError,        ///< Error due to TCP socket.
        KeepAliveError,      ///< Error due to no response to a keep alive.
        LoginFail,           ///< 登录失败  
        NotAuthorized,       ///< 验证错误  
        Conflict,            ///< 用户已经注册  
        OtherError,          ///< 其它错误  
        InternalServerError  ///< 服务器内部错误  
    };

signals:
    /**
     * @brief 用户成功登录后触发，界面可以初始化  
     * @see Login
     */
    void sigClientConnected();
    /**
     * @brief 用户登出后触发，通知界面析构  
     * @see Logout
     */
    void sigClientDisconnected();
    /**
     * @brief 登录失败触发的信号，通知界面处理错误  
     * @param e:错误类型  
     * @see ERROR_TYPE
     * @see Login
     * @see Logout
     * @see Register
     */
    void sigClientError(CClient::ERROR_TYPE e);

    /**
     * @brief 从存储（文件）中加载好友完成后触发，通知界面初始化数据  
     *
     */
    void sigLoadRosterFromStorage();
    /**
     * @brief 更新本地用户信息后触发。好友的数据已经更新，调用 USER_INFO_LOCALE 访问本地用户信息  
     * @see RequestUserInfoLocale
     * @see USER_INFO_LOCALE
     * @see CUser
     * @see CManageUser::GetUserInfoLocale()
     */
    void sigUpdateLocaleUserInfo();
    /**
     * @brief 更新好友信息后触发.新增好友会触发此消息.好友列表显示控件，
     *        根据好友列表中是否有此好友来决定是否插入或更新好友。
     *        调用 GLOBAL_USER->GetUserInfoRoster 来访问指定的好友信息  
     *
     * @param szId：好友ID  
     * @param user：好友（CUser）信息对象  
     * @see RequestUserInfoRoster  
     * @see CUser
     * @see CManageUser
     */
    void sigUpdateRosterUserInfo(const QString &szId, QSharedPointer<CUser> user);
    /**
     * @brief 删除好友时触发  
     *
     * @param szId：好友ID  
     */
    void sigRemoveRosterUserInfo(const QString &szId);
    /**
     * @brief 好友状态改变时触发  
     *
     * @param szId:好友ID  
     * @see setClientStatus
     */
    void sigChangedStatus(const QString& szId);

    /**
     * @brief 接收增加好友请求时触发  
     *
     * @param szId：请求者ID  
     * @param type：订阅类型 @see SUBSCRIBE_TYPE
     */
    void sigRosterAddReceived(const QString& szId, const CClient::SUBSCRIBE_TYPE &type);
    /**
     * @brief 删除好友请求时触发  
     *
     * @param szId：删除好友的ID  
     * @see sigRosterRemoveReceived
     */
    void sigRosterRemoveReceived(const QString& szId);

    /**
     * @brief 接收到新消息时触发  
     *
     * @param szId:好友ID  
     * @see SendMessage
     */
    void sigMessageUpdate(const QString& szId);

    /**
     * @brief 清空未读消息时触发  
     *
     * @param szId:好友ID  
     */
    void sigMessageClean(const QString& szId);

    /**
     * @brief 文件接收时触发  
     *
     * @param szId:用户ID  
     * @param file
     */
    void sigFileReceived(const QString &szId, QSharedPointer<CFileTransfer> file);

protected slots:
    /**
     * @brief 客户端连接后进行初始化工作.注意:先初始化数据,再初始化界面  
     *
     */
    virtual void slotClientConnected(const QString &szId);
    /**
     * @brief 客户端断开连接时清理操作.注意:先清理界面,再清理数据  
     *
     */
    virtual void slotClientDisconnected();

protected:
    virtual int SetLogin(bool bLogin);
private:
    bool m_bIsLogin;
};

#endif // CLIENT_H
