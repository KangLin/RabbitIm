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
     * @brief 视频呼叫  
     *
     * @param szId：用户ID  
     * @return int
     */
    virtual int CallVideo(QString szId);

signals:
    
public slots:
    void slotCallVideoReceived(QSharedPointer<CCallObject> call);

private:
    QSharedPointer<CCallObject> m_Call;
};

#endif // MANAGECALL_H
