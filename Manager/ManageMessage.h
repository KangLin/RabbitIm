#ifndef MANAGEMESSAGE_H
#define MANAGEMESSAGE_H

#include <QObject>
#include "Message/Message.h"

class CManageMessage : public QObject
{
    Q_OBJECT
public:
    explicit CManageMessage(QObject *parent = 0);

    int Init(const QString &szId);
    int Clean();
    CMessage* GetMessage(const QString &szId);
signals:
    
public slots:
    
};

#endif // MANAGEMESSAGE_H
