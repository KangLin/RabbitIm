#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDate>
#include <QSharedPointer>
#include "ChatActions/ChatAction.h"
#include <vector>

class CMessage : public QObject
{
    Q_OBJECT
public:
    explicit CMessage(QObject *parent = 0);
    virtual ~CMessage();

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
