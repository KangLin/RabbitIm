#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include <QObject>
#include <QVideoFrame>
#include "qxmpp/QXmppRtpChannel.h"
#include "../../Tool.h"

class CFrameProcess : public QObject
{
    Q_OBJECT
public:
    explicit CFrameProcess(QObject *parent = 0);
    virtual ~CFrameProcess();

signals:
    //帧被处理完后的信号
    void sigCaptureFrame(const QVideoFrame &frame);
    //转换视频帧格式后触发的信号，用于视频压缩发送
    void sigConvertedToYUYVFrame(const QXmppVideoFrame &frame);

    //转换到RGB32格式的帧
    void sigConvertedToRGB32Frame(const QVideoFrame &frame);

public slots:
    //处理帧的槽，因为不同的平台，捕获的帧方向不一样，需要做转换
    void slotCaptureFrame(const QVideoFrame &frame);
    //转换到YUYV格式
    void slotFrameConvertedToYUYV(const QVideoFrame &frame, int nWidth = 320, int nHeight = 240);

    //转换帧格式到RGB32
    void slotFrameConvertedToRGB32(const QVideoFrame &inFrame, const QRect &rect);
    void slotFrameConvertedToRGB32(const QXmppVideoFrame &frame, const QRect &rect);

private:
    //用于从AVPICTURE输出到QVideoFrame中
    int FillFrame(const AVPicture &pic, const QRect &rect, QVideoFrame &frame);

};

#endif // VIDEOPROCESS_H
