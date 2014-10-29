#ifndef MANAGECALL_H
#define MANAGECALL_H

#include <QObject>
#include "CallObject.h"

class CManageCall : public QObject
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
    virtual int Init(QString szId);
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
    virtual int ProcessCommandCall(const QString &szId, const QString &szCommand);

signals:
    
private slots:
    void slotCallVideoReceived(QSharedPointer<CCallObject> call);
    void slotCallFinished(QSharedPointer<CCallObject> call);

private:
    QSharedPointer<CCallObject> m_Call;
    bool m_bVideoCall;
};

#endif // MANAGECALL_H
