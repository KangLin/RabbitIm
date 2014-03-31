#ifndef FRMVIDEO_H
#define FRMVIDEO_H

#include <QFrame>
#include "../../XmppClient.h"
#include <QAudioOutput>
#include <QAudioInput>
#include <QBuffer>

class CRoster;

namespace Ui {
class CFrmVideo;
}

class CFrmVideo : public QFrame
{
    Q_OBJECT

public:
    explicit CFrmVideo(QWidget *parent = 0);
    ~CFrmVideo();

    int SetClient(CXmppClient *pClient);
    int Call(QString jid);

protected slots:
    //有被呼叫时触发（只有被叫方才有）
    void callReceived(QXmppCall* pCall);
    //调用开始时触发
    void callStarted(QXmppCall *pCall);

    //对方正在响铃时触发（只有呼叫方才有）
    void ringing();

    //呼叫状态改变时触发
    void stateChanged(QXmppCall::State state);

    //呼叫已链接时触发
    //当这个信号触发后,可以边接QAudioOutput和QAudioInput到呼叫
    //你可以调用通道的payloadType()来决定音频的参数（时钟、通道数等）
    void connected();

    //音频信道有改变时触发
    void audioModeChanged(QIODevice::OpenMode mode);

    //视频信道有改变时触发
    void videoModeChanged(QIODevice::OpenMode mode);

    //当呼叫结束时触摸发
    ///
    //注意：在这个函数中不能直接删除呼叫(call)
    //用deleteLater()代替。
    void finished();

private:
    Ui::CFrmVideo *ui;

    QXmppCall* m_pCall;
    CXmppClient *m_pClient;

    QAudioInput*        m_pAudioInput;
    QAudioOutput*       m_pAudioOutput;
};

#endif // FRMVIDEO_H
