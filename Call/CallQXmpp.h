#ifndef CALLQXMPP_H
#define CALLQXMPP_H

#include "qxmpp/QXmppCallManager.h"
#include "CallVideo.h"
#include <QAudioInput>
#include <QAudioOutput>

class CCallQXmpp : public CCallObject
{
    Q_OBJECT
public:
    explicit CCallQXmpp(QXmppCall* pCall, QObject *parent = 0);
    virtual ~CCallQXmpp();

    virtual int Accept();
    virtual int Cancel();

    virtual State GetState();

protected:
    //连接与 call 相关的信号  
    virtual int ConnectionCallSlot(QXmppCall *pCall);
    
private slots:
    virtual void slotConnection();
    virtual void slotStateChanged(QXmppCall::State state);
    virtual void slotFinished();
    virtual void slotAudioModeChanged(QIODevice::OpenMode mode);

private:
    int StartAudioDevice();
    int StopAudioDevice();
    int OpenAudioInput();
    int OpenAudioOutput();

private:
    QAudioInput*  m_pAudioInput; //音频输入设备  
    QAudioOutput* m_pAudioOutput;//音频输出设备  

protected:
    QXmppCall* m_pCall;
};

#endif // CALLQXMPP_H
