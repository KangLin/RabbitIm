#ifndef CHATACTIONGROUPCHAT_H
#define CHATACTIONGROUPCHAT_H

#include <QObject>
#include "Message/ChatActions/MessageAction.h"
#include "Global/GlobalDir.h"

class RABBITIM_SHARED_LIBRARY CChatActionGroupChat : public CMessageAction
{
    Q_OBJECT
public:
    /*
     *  szId:房间ID  
     * message:发送的消息  
     * szSendId:发送消息的人的ID  
     * date:消息产生时间  
     * me:是否是本人产生的消息  
     */
    CChatActionGroupChat(const QString &szId, const QString &message, const QString &szSendId, const QTime &time, const bool &me);
    virtual QString getContent();
signals:
    
public slots:
    
private:
    QString m_szSendId;
};

#endif // CHATACTIONGROUPCHAT_H
