#ifndef CALLQXMPP_H
#define CALLQXMPP_H

#include "QXmppCallManager.h"
#include <QAudioInput>
#include <QAudioOutput>
#include "Call/CallObject.h"
#include <QObject>
#include <QTimer>
#include <QThread>

/**
 * @brief 呼叫qxmpp实现类  
 * @ingroup RABBITIM_IMPLEMENT_QXMPP
 */
class CCallObjectQXmpp : public CCallObject
{
    Q_OBJECT

public:
    explicit CCallObjectQXmpp(QXmppCall* pCall, bool bVideo = false,
                              QObject *parent = nullptr);
    virtual ~CCallObjectQXmpp();

public slots:
    virtual int Accept();
    virtual int Stop(StopState state = Reject);

protected:
    //连接与 call 相关的信号  
    virtual int ConnectionCallSlot(QXmppCall *pCall);

protected Q_SLOTS:
    virtual void slotConnection();
    virtual void slotStateChanged(QXmppCall::State state);
    virtual void slotFinished();

//音频  
protected slots:
    virtual void slotAudioModeChanged(QIODevice::OpenMode mode);

private:
    int StartAudioDevice();
    
//视频    
protected slots:
    virtual void slotVideoModeChanged(QIODevice::OpenMode mode);
    //发送视频帧到网络  
    void soltVideoFrameToRemote(const QVideoFrame &frame);
    //从网络接收视频帧  
    void slotReciveFrame();

private:
    int SetVideoFormat();
    int StartVideo();
    int StopVideo();

private:
    QTimer m_tmRecive;

    QXmppCall* m_pCall;
};

#endif // CALLQXMPP_H
