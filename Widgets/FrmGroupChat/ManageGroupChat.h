#ifndef MANAGEGROUPCHAT_H
#define MANAGEGROUPCHAT_H

#include <QObject>
#include <QSharedPointer>
#include "GroupChat.h"

class CManageGroupChat : public QObject
{
    Q_OBJECT
public:
    explicit CManageGroupChat(QObject *parent = 0);

    virtual int Create(const QString &szId, 
                   const QString &szName,
                   const QString &szSubject,
                   const QString szPassword = QString(),
                   bool bProtracted = false
            ) = 0;
    virtual int Join(const QString &szId) = 0;
    virtual QSharedPointer<CGroupChat> Get(const QString &szId) = 0;

signals:
    
public slots:
    
};

#endif // MANAGEGROUPCHAT_H
