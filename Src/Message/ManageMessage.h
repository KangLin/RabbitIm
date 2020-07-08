#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDate>
#include <QSharedPointer>
#include "ChatActions/ChatAction.h"
#include <vector>
#include "Global/GlobalDir.h"

/**
 * @defgroup RABBITIM_INTERFACE_MANAGEMESSAGE 消息管理接口类模块  
 * @ingroup RABBITIM_INTERFACE_USER
 * @brief 消息管理接口类模块。用于管理好友消息。  
 */

/**
 * @class CManageMessage
 * @ingroup RABBITIM_INTERFACE_MANAGEMESSAGE RABBITIM_INTERFACE 
 * @brief 消息管理接口类。用于管理好友消息。  
 * @see CUser
 */
class RABBITIM_SHARED_LIBRARY CManageMessage : public QObject
{
    Q_OBJECT
public:
    explicit CManageMessage(QObject *parent = 0);
    virtual ~CManageMessage();

    ///< 得到未读消息数  
    int GetUnReadCount();
    ///< 增加消息  
    QSharedPointer<CChatAction> AddMessage(const QString& szId, 
                                           const QString& szMessage,
                                           const bool bMe = false, 
                                           const QTime time = QTime::currentTime());
    int AddMessage(QSharedPointer<CChatAction> chatAction);

    ///< 读取消息  
    ///< 读取所有未读消息  
    std::vector<QSharedPointer<CChatAction> > GetUnreadMessage();

signals:
public slots:
private:
    std::vector<QSharedPointer<CChatAction> > m_Message;
    int m_nUnreadCount;//未读消息数  
};

#endif // MESSAGE_H
