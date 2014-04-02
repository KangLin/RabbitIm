/*
 * 作者：康林(msn、email: kl222@126.com)
 *
 * 从摄像头（QCarmera）或者（Player）中捕获视频帧。
 * 用法：
 *      QCamera m_Camera;
 *      m_Camera.setViewfinder(&m_CaptureVideoFrame);
 *      m_Camera.setCaptureMode(QCamera::CaptureVideo);
 * 注册SLOT：
 *      connect(&m_frame, SIGNAL(CaptureFrame(const QVideoFrame&)),
 *           SLOT(CaptureVideoFrame(const QVideoFrame&)));
 * 在SLOT 中 CaptureVideoFrame(const QVideoFrame&) 处理捕获到的视频帧。
 *
 */

#ifndef CAPTUREVIDEOFRAME_H
#define CAPTUREVIDEOFRAME_H

#include <QAbstractVideoSurface>

class CCaptureVideoFrame : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit CCaptureVideoFrame(QObject *parent = 0);

    virtual QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

    virtual bool present(const QVideoFrame &frame);

signals:
     void CaptureFrame(const QVideoFrame &frame);

public slots:

};

#endif // CAPTUREVIDEOFRAME_H
