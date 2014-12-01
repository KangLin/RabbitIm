#ifndef CHATACTIONGROUPCHAT_H
#define CHATACTIONGROUPCHAT_H

#include <QObject>
#include "Message/ChatActions/MessageAction.h"

class CChatActionGroupChat : public CMessageAction
{
    Q_OBJECT
public:
    CChatActionGroupChat(const QString &szId, const QString &message, const QString &szSendId, const QTime &date, const bool &me);
    virtual QString getContent();
signals:
    
public slots:
    
private:
    QString m_szSendId;
};

#endif // CHATACTIONGROUPCHAT_H
