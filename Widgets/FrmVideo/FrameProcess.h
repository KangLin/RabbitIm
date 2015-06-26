#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include <QObject>
#include <QVideoFrame>
#include "qxmpp/QXmppRtpChannel.h"
#include "../../Tool.h"
#include "Media/Camera/Camera.h"

class CCameraQt;
class CFrameProcess : public QObject
{
    Q_OBJECT
public:
    explicit CFrameProcess(QObject *parent = 0);
    virtual ~CFrameProcess();

public slots:
    //摄像头捕获帧处理槽，因为不同的平台，捕获的帧方向不一样，需要做转换  
    void slotCaptureFrame(const std::shared_ptr<CVideoFrame> frame);
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
    //转换视频帧格式后触发的信号，由函数slotFrameConvertedToYUYV转换完成后触发,用于视频压缩发送  
    void sigFrameConvertedToYUYVFrame(const QXmppVideoFrame &frame);
    //转换到RGB32格式的帧,由函数slotFrameConvertedToRGB32转换完成后触发  
    void sigFrameConvertedToRGB32Frame(const QVideoFrame &frame);

private:
#ifdef RABBITIM_USE_FFMPEG
    //用于从AVPICTURE输出到QVideoFrame中  
    int FillFrame(const AVPicture &pic, const QRect &rect, QVideoFrame &frame);
    int FillFrame(const std::shared_ptr<CVideoFrame> inFrame, QVideoFrame &outFrame);
#endif
};

#endif // VIDEOPROCESS_H
