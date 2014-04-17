#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include <QObject>
#include <QVideoFrame>
#include "qxmpp/QXmppRtpChannel.h"

class CCaptureFrameProcess : public QObject
{
    Q_OBJECT
public:
    explicit CCaptureFrameProcess(QObject *parent = 0);
    virtual ~CCaptureFrameProcess();

signals:
    //帧被处理完后的信号
    void sigCaptureFrame(const QVideoFrame &frame);
    //转换视频帧格式后触发的信号，用于视频压缩发送
    void sigConvertedToYUYVFrame(const QXmppVideoFrame &frame);

public slots:
    //处理帧的槽
    void slotCaptureFrame(const QVideoFrame &frame);
    //转换到YUYV格式
    void slotFrameConvertedToYUYV(const QVideoFrame &frame, int nWidth = 320, int nHeight = 240);

};

#endif // VIDEOPROCESS_H
