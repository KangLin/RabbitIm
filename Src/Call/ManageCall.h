#ifndef MANAGECALL_H
#define MANAGECALL_H

#include <QObject>
#include <QSharedPointer>
#include <QVideoFrame>
#include "CallObject.h"
#include "Manage/Manage.h"
#include "Global/GlobalDir.h"

/**
  *@defgroup RABBITIM_INTERFACE_MANGECALL 呼叫管理类模块  
  *@ingroup  RABBITIM_INTERFACE_MANAGER
  *@brief    呼叫管理类模块
  */

/**
 * @ingroup RABBITIM_INTERFACE_MANGECALL RABBITIM_INTERFACE
 * @brief   呼叫管理类接口,用于管理多个呼叫对象（CCallObject）
 *          和其相关的界面通信接口。\n
 *          在此类中实现音频、视频捕获。
 */
class RABBITIM_SHARED_LIBRARY CManageCall : public CManage
{
    Q_OBJECT

public:
    explicit CManageCall(QObject *parent = nullptr);
    virtual ~CManageCall();

    /**
     * @brief 用户登录成功后调用,用于初始化工作，如果要重载它，  
     * 则需要在重载函数中调用 CManageCall::LoginInit(szId);   
     *
     * @param szId:登录用户名  
     * @return int
     */
    virtual int LoginInit(const QString &szId);
    /**
     * @brief 用户登出时调用,用于清理工作。如果要重载它，  
     * 则需要在重载函数中调用 CManageCall::LogoutClean();    
     *
     * @return int
     */
    virtual int LogoutClean();

public Q_SLOTS:
    /**
     * @brief 主动呼叫  
     *
     * @param szId：用户ID  
     * @param bVideo:是否是视频呼叫  
     * @return int
     */
    virtual int Call(const QString &szId, bool bVideo = false);
    /**
     * @brief 接收呼叫  
     * @param szId:用户ID
     * @return 
     */
    virtual int Accept(QString szId);
    /**
     * @brief 结束呼叫  
     * @param szId:用户ID
     * @return 
     * @see slotRosterStatusChanged
     */
    virtual int Stop(QString szId);
    /**
     * @brief 判断呼叫是否正在进行  
     * @param szId:用户ID
     * @return 
     */
    virtual bool IsRun(QString szId);

    /**
     * @brief 根据命令串执行操作  
     * @param szId:用户ID  
     * @param szCommand: accept、cancel、call
     * @return int
     * @see CFrmMessage::slotAnchorClicked
     * @see CCallAction:包含命令  
     */
    virtual int ProcessCommandCall(const QString &szId, const QString &szCommand);
   
private:
    /**
     * @brief 具体协议实现呼叫,主要是根据协议生成CCallObject实例    
     * @param szId：用户 Id  
     * @param call:如果成功,包含新建的呼叫实例  
     * @param bVideo：是否包含视频  
     * @return 成功返回0，失败返回非0  
     * @see CManageCall::Call
     */
    virtual int OnCall(const QString &szId,
              /*[out]*/QSharedPointer<CCallObject> &call,
              /*[in]*/ bool bVideo = false);

protected Q_SLOTS:
    /**
     * @brief 用于完成接收到呼叫消息的动作。  
     *        由具体的协议调用此方法。
     * @param call
     */
    void slotCallReceived(QSharedPointer<CCallObject> call);

private Q_SLOTS:
    /**
     * @brief 呼叫结束处理  
     * @param pCall
     */
    void slotCallFinished(CCallObject* pCall);
    /*
     * @brief 此函数完成当正在视频通话时，好友直接 Logout 操作,  
     *        因为 LOGOUT 操作发生在 CManageCall::LogoutClean() 前，  
     *        所以收到不视频的停止信令，所以需要在此监控好友的 OFFLINE  
     *        状态来关闭视频    
     * @param szId: 用户ID  
     * @see   CManageCall::LogoutClean() CClient::slotClientDisconnected
     */
    void slotRosterStatusChanged(const QString &szId);

protected:
    /**
     * @brief 已有的呼叫 
     */
    QMap<QString, QSharedPointer<CCallObject> > m_Call;
};

#endif // MANAGECALL_H
