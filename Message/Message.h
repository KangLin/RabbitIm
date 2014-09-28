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

    ///< 得到未读消息数  
    int GetUnReadCount();
    ///< 增加消息  
    QSharedPointer<CChatAction> AddMessage(const QString& szId, const QString& szMessage, const QDate date,  const bool bMe = false) const;
    ///< 读取消息  
    ///< 读取所有未读消息  

signals:
    
public slots:
private:
    std::vector<QSharedPointer<CChatAction> > m_Message;
};

#endif // MESSAGE_H
