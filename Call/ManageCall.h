#ifndef MANAGECALL_H
#define MANAGECALL_H

#include <QObject>
#include <QSharedPointer>
#include "CallObject.h"
#include "Manage/Manage.h"

/**
  *@defgroup RABBITIM_INTERFACE_MANGECALL 呼叫管理类模块  
  *@ingroup RABBITIM_INTERFACE_MANAGER
  *@brief 呼叫管理类模块  
  */

/**
 * @ingroup RABBITIM_INTERFACE_MANGECALL RABBITIM_INTERFACE
 * @brief 呼叫管理类接口  
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

    /**
     * @brief 呼叫  
     *
     * @param szId：用户ID  
     * @param bVideo:是否是视频呼叫  
     * @return int
     */
    virtual int Call(QString szId, bool bVideo = false);

    virtual bool IsRun();
    virtual int Stop();
    /**
     * @brief 根据命令串执行操作  
     *
     * @param szId
     * @param szCommand: accept、cancel、call
     * @return int
     * @see CFrmMessage::slotAnchorClicked
     */
    virtual int ProcessCommandCall(const QString &szId, const QString &szCommand);

signals:
    
private slots:
    void slotCallVideoReceived(QSharedPointer<CCallObject> call);
    void slotCallFinished(CCallObject* pCall);

private:
    QSharedPointer<CCallObject> m_Call;
    bool m_bVideoCall;
};

#endif // MANAGECALL_H
