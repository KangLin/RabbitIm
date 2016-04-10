#ifndef CALLQXMPP_H
#define CALLQXMPP_H

#include "qxmpp/QXmppCallManager.h"
#include <QAudioInput>
#include <QAudioOutput>
#include "Call/CallObject.h"
#include <QObject>
#include <QTimer>
#include <QThread>
#include "Widgets/FrmVideo/FrameProcess.h"
#include "Widgets/FrmVideo/FrmPlayer.h"
#include "Widgets/FrmVideo/FrmVideo.h"
#include "Media/Camera/CameraFactory.h"

/**
 * @brief 呼叫qxmpp实现类  
 * @ingroup RABBITIM_IMPLEMENT_QXMPP
 */
class CCallObjectQXmpp : public CCallObject, CCamera::CHanderFrame
{
    Q_OBJECT

public:
    explicit CCallObjectQXmpp(QXmppCall* pCall, bool bVideo = false, QObject *parent = 0);
    virtual ~CCallObjectQXmpp();

public slots:
    virtual int Accept();
    virtual int Stop();

protected:
    //连接与 call 相关的信号  
    virtual int ConnectionCallSlot(QXmppCall *pCall);

protected slots:
    virtual void slotConnection();
    virtual void slotStateChanged(QXmppCall::State state);
    virtual void slotFinished();
    
//音频  
protected slots:
    virtual void slotAudioModeChanged(QIODevice::OpenMode mode);

private:
    int StartAudioDevice();
    int StopAudioDevice();

private:
    QAudioInput*  m_pAudioInput; ///< 音频输入设备  
    QAudioOutput* m_pAudioOutput;///< 音频输出设备  
    
//视频    
protected slots:
    virtual void slotVideoModeChanged(QIODevice::OpenMode mode);
    //发送视频帧到网络  
    void slotCaptureFrame(const QVideoFrame &frame);
    //从网络接收视频帧  
    void slotReciveFrame();

    //配置更新  
    void slotUpdateOption();

private:
    int SetVideoFormat();
    int StartVideo();
    int StopVideo();
    int OpenVideoWindow();
    virtual int OnFrame(const QVideoFrame &frame);

    int ConnectLocaleVideo();
    int DisconnectLocaleVideo();

private:
    QTimer m_tmRecive;
    CFrameProcess m_CaptureFrameProcess; //本地显示  
    CFrameProcess m_ReciveFrameProcess;   //从网络接收  
    QThread m_VideoThread;
    CCamera* m_pCamera;
    
    QXmppCall* m_pCall;
};

#endif // CALLQXMPP_H
