#ifndef MANAGECALL_H
#define MANAGECALL_H

#include <QObject>
#include <QSharedPointer>
#include <QVideoFrame>
#include "CallObject.h"
#include "Manage/Manage.h"

/**
  *@defgroup RABBITIM_INTERFACE_MANGECALL 呼叫管理类模块  
  *@ingroup RABBITIM_INTERFACE_MANAGER
  *@brief 呼叫管理类模块  
  */

/**
 * @ingroup RABBITIM_INTERFACE_MANGECALL RABBITIM_INTERFACE
 * @brief 呼叫管理类接口,用于管理多个呼叫对象（CCallObject）  
 *        和其相关的界面通信接口。\n
 *        在此类中实现音频、视频捕获。   
 */
class CManageCall : public CManage
{
    Q_OBJECT

public:
    explicit CManageCall(QObject *parent = 0);
    virtual ~CManageCall();

    /**
     * @brief 用户登录成功后调用,用于初始化工作  
     *
     * @param szId:登录用户名  
     * @return int
     */
    virtual int Init(const QString &szId);
    /**
     * @brief 用户登出时调用,用于清理工作   
     *
     * @return int
     */
    virtual int Clean();

public slots:
    /**
     * @brief 呼叫  
     *
     * @param szId：用户ID  
     * @param bVideo:是否是视频呼叫  
     * @return int
     */
    virtual int Call(const QString &szId, bool bVideo = false);
    virtual int Accept(QString szId);
    virtual int Stop(QString szId);
    virtual bool IsRun(QString szId);

    /**
     * @brief 根据命令串执行操作  
     * @param szId
     * @param szCommand: accept、cancel、call
     * @return int
     * @see CFrmMessage::slotAnchorClicked
     * @see CCallAction:包含命令  
     */
    virtual int ProcessCommandCall(const QString &szId, const QString &szCommand);

signals:
    /// 呼叫状态更新时触发  
    void sigStateUpdate(const QString &szId);
    /// 呼叫结束时触发  
    void sigFinished(const QString &szId);
    /// 本地帧触发信号,此信号用于本地视频显示窗口  
    void sigLocalVideoFrame(const QString &szId, const QVideoFrame &frame);
    /// 远程帧触发信号，此信号用于远程显示窗口  
    void sigRemoteVideoFrame(const QString &szId, const QVideoFrame &frame);

protected slots:
    void slotCallReceived(QSharedPointer<CCallObject> call);
    void slotCallFinished(CCallObject* pCall);

private:
    /**
     * @brief 具体协议实现呼叫  
     * @param szId：用户 Id  
     * @param bVideo：是否包含视频  
     * @return 成功返回0，失败返回非0  
     * @see CManageCall::Call
     */
    virtual int OnCall(const QString &szId, bool bVideo = false) = 0;

protected:
    /**
     * @brief 已有的呼叫 
     */
    QMap<QString, QSharedPointer<CCallObject> > m_Call;
};

#endif // MANAGECALL_H
