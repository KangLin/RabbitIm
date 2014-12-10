#ifndef USER_H
#define USER_H

#include <QObject>
#include <QSharedPointer>
#include "UserInfo.h"
#include "Message/ManageMessage.h"

#undef GetMessage

class CUser : public QObject
{
    Q_OBJECT
public:
    explicit CUser(QObject *parent = 0);
    virtual ~CUser();

    QSharedPointer<CUserInfo> GetInfo();
    QSharedPointer<CManageMessage> GetMessage();

signals:

public slots:
private:
    QSharedPointer<CUserInfo> m_Info;
    QSharedPointer<CManageMessage> m_Message;
};

#endif // USER_H
