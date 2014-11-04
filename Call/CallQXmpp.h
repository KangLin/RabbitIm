#ifndef CALLQXMPP_H
#define CALLQXMPP_H

#include "qxmpp/QXmppCallManager.h"
#include <QAudioInput>
#include <QAudioOutput>
#include "CallObject.h"
#include <QObject>
#include <QTimer>
#include <QThread>
#include "CallQXmpp.h"
#include "Widgets/FrmVideo/FrameProcess.h"
#include "Widgets/FrmVideo/FrmVideo.h"

#if ANDROID && RABBITIM_USER_OPENCV
    #include "Widgets/FrmVideo/CameraOpencv.h"
#else
    #include "Widgets/FrmVideo/Camera.h"
#endif

class CCallQXmpp : public CCallObject
{
    Q_OBJECT
public:
    explicit CCallQXmpp(QXmppCall* pCall, bool bVideo = false, QObject *parent = 0);
    virtual ~CCallQXmpp();

public slots:
    virtual int Accept();
    virtual int Cancel();

public:
    virtual State GetState();

protected:
    //连接与 call 相关的信号  
    virtual int ConnectionCallSlot(QXmppCall *pCall);
    
protected slots:
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

//视频    
protected slots:
    virtual void slotVideoModeChanged(QIODevice::OpenMode mode);
    //发送视频帧到网络  
    void slotCaptureFrame(const QXmppVideoFrame& frame);
    //从网络接收视频帧  
    void slotReciveFrame();
    
    void slotFrmVideoClose();

private:
    int SetVideoFormat();
    int StartVideo();
    int StopVideo();

private:
    QTimer m_tmRecive;
    CFrameProcess m_CaptureFrameProcess;
    CFrameProcess m_CaptureToRemoteFrameProcess;
    CFrameProcess m_ReciveFrameProcess;
    QThread m_VideoThread;
#if ANDROID && RABBITIM_USER_OPENCV
    CCameraOpencv m_Camera;
#else
    CCamera m_Camera;
#endif
    CFrmVideo *m_pFrmVideo;
};

#endif // CALLQXMPP_H
