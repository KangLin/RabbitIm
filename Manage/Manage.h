#ifndef MANAGE_H
#define MANAGE_H

#include <QObject>

/**
 * @brief 所有管理接口的基类。  
 * @ingroup RABBITIM_INTERFACE
 */
class CManage : public QObject
{
    Q_OBJECT
public:
    explicit CManage(QObject *parent = 0);

    /**
     * @brief 用户登录成功后调用,用于初始化工作  
     *
     * @param szId:登录用户名  
     * @return int
     */
    virtual int Init(const QString &szId) = 0;
    /**
     * @brief 用户登出时调用,用于清理工作   
     *
     * @return int
     */
    virtual int Clean() = 0;

signals:
    
public slots:
    
};

#endif // MANAGE_H
