#ifndef CALLVIDEOQXMPP_H
#define CALLVIDEOQXMPP_H

#include <QObject>
#include "CallQXmpp.h"

class CCallVideoQXmpp : public CCallQXmpp
{
    Q_OBJECT
public:
    explicit CCallVideoQXmpp(QXmppCall* pCall, QObject *parent = 0);
    
protected:
    //连接与 call 相关的信号  
    virtual int ConnectionCallSlot(QXmppCall *pCall);
signals:
    
private slots:
    virtual void slotVideoModeChanged(QIODevice::OpenMode mode);

};

#endif // CALLVIDEOQXMPP_H
