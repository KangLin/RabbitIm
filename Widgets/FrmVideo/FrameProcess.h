#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include <QObject>
#include <QVideoFrame>
#include "common/Tool.h"
#ifdef RABBITIM_USE_QXMPP
    #include "qxmpp/QXmppRtpChannel.h"
#endif
#include "Global/GlobalDir.h"

class RABBITIM_SHARED_LIBRARY CFrameProcess : public QObject
{
    Q_OBJECT
public:
    explicit CFrameProcess(QObject *parent = 0);
    virtual ~CFrameProcess();

public slots:
    //摄像头捕获帧处理槽，因为不同的平台，捕获的帧方向不一样，需要做转换  
    void slotCaptureFrame(const QVideoFrame &frame);
    //转换到YUYV格式,转换完成后,触发信号 sigFrameConvertedToYUYVFrame
    void slotFrameConvertedToYUYV(const QVideoFrame &frame,
                                  int nWidth = 320, int nHeight = 240);
    //转换帧格式到RGB32,转换完成后,触发信号sigConvertedToRGB32Frame  
    void slotFrameConvertedToRGB32(
            const QVideoFrame &frame, QRect rect = QRect());
#ifdef RABBITIM_USE_QXMPP
    void slotFrameConvertedToRGB32(
            const QXmppVideoFrame &frame, QRect rect = QRect());
#endif

signals:
    //摄像头捕获帧被处理完后的信号  
    void sigCaptureFrame(const QVideoFrame &frame);
#ifdef RABBITIM_USE_QXMPP
    //转换视频帧格式后触发的信号，由函数slotFrameConvertedToYUYV转换完成后触发,用于视频压缩发送  
    void sigFrameConvertedToYUYVFrame(const QVideoFrame &frame);
#endif
    //转换到RGB32格式的帧,由函数slotFrameConvertedToRGB32转换完成后触发  
    void sigFrameConvertedToRGB32Frame(const QVideoFrame &frame);
};

#endif // VIDEOPROCESS_H
