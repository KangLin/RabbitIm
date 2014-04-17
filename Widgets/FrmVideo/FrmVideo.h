#ifndef FRMVIDEO_H
#define FRMVIDEO_H

#include <QFrame>
#include <QCamera>
#include <QAudioOutput>
#include <QAudioInput>
#include <QBuffer>
#include <QTimer>
#include "CaptureVideoFrame.h"
#include "FrmPlayer.h"
#include "../../XmppClient.h"

class CRoster;

namespace Ui {
class CFrmVideo;
}

class CFrmVideo : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmVideo(QWidget *parent = 0);
    virtual ~CFrmVideo();

    int SetClient(CXmppClient *pClient);
    int Call(QString jid);

protected slots:
    //有被呼叫时触发（只有被叫方才有）
    void callReceived(QXmppCall* pCall);
    //呼叫开始时触发
    void callStarted(QXmppCall *pCall);

    //对方正在响铃时触发（只有呼叫方才有）
    void ringing();

    //呼叫状态改变时触发
    void stateChanged(QXmppCall::State state);

    //呼叫会话建立时触发，这个是信令通道
    //当这个信号触发后,可以边接QAudioOutput和QAudioInput到呼叫
    //你可以调用通道的payloadType()来决定音频的参数（时钟、通道数等）
    //这时可以调用QXmppCall::startVideo开始视频
    void connected();

    //ICE连接建立后，音频信道有改变时触发，这个是数据通道
    void audioModeChanged(QIODevice::OpenMode mode);

    //ICE连接建立后，视频信道有改变时触发，这个是数据通道
    void videoModeChanged(QIODevice::OpenMode mode);

    //当呼叫结束时触摸发
    //注意：在这个函数中不能直接删除呼叫(call)
    //用deleteLater()代替。
    void finished();

    void clientIqReceived(const QXmppIq &iq);

private:
    int StopDevice();
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *event);

    //调整显示窗体大小
    void AdjustPlayer(const QRect &rect);

private:
    Ui::CFrmVideo *ui;

    bool m_bCall;//是否是主叫方，用于开始视频m_pCall->startVideo
    QXmppCall* m_pCall;
    CXmppClient *m_pClient;

    QAudioInput*        m_pAudioInput;
    QAudioOutput*       m_pAudioOutput;

    QCamera m_Camera;
    CCaptureVideoFrame m_CaptureVideoFrame;  //实现捕获视频帧
    CFrmPlayer m_RemotePlayer;//远程视频播放窗口
    CFrmPlayer m_LocalePlayer;//本地视频播放窗口
    QTimer m_Timer;
    int StartVideo();
    int StopVideo();


private slots:
    //用于向qxmpp输出视频幀
    void slotCaptureFrame(const QXmppVideoFrame &frame);
    void slotUpdateReciverVideo();

};

#endif // FRMVIDEO_H
