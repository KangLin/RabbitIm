#ifndef MANAGEGROUPCHATQXMPP_H
#define MANAGEGROUPCHATQXMPP_H

#include <QObject>
#include <QMap>
#include "GroupChat.h"
#include "ManageGroupChat.h"

class CManageGroupChatQxmpp : public CManageGroupChat
{
    Q_OBJECT
public:
    explicit CManageGroupChatQxmpp(QObject *parent = 0);

    virtual int Create(const QString &szId, 
                   const QString &szName,
                   const QString &szSubject,
                   const QString szPassword = QString(),
                   bool bProtracted = false
            );
    virtual int Join(const QString &szId);
    virtual QSharedPointer<CGroupChat> Get(const QString &szId);

signals:
    
public slots:
private:
    QMap<QString, QSharedPointer<CGroupChat> > m_GroupChat;
};

#endif // MANAGEGROUPCHATQXMPP_H
