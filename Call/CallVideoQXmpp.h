#ifndef CALLVIDEOQXMPP_H
#define CALLVIDEOQXMPP_H
#include "qxmpp/QXmppCallManager.h"
#include "CallVideo.h"

class CCallVideoQXmpp : public CCallVideo
{
public:
    explicit CCallVideoQXmpp(QXmppCall* pCall, QObject *parent = 0);

    virtual int Accept();
    virtual int Cancel();

    virtual State GetState();

private:
    //连接与 call 相关的信号  
    int ConnectionCallSlot(QXmppCall *pCall);
private slots:
    void slotConnection();
    void slotStateChanged(QXmppCall::State state);
    void slotFinished();
    void slotAudioModeChanged(QIODevice::OpenMode mode);
    void slotVideoModeChanged(QIODevice::OpenMode mode);

private:
    QXmppCall* m_pQXmppCall;
};

#endif // CALLVIDEOQXMPP_H
