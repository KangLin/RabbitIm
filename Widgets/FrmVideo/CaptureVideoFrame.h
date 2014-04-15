/*
 * 作者：康林(msn、email: kl222@126.com)
 *
 * 从摄像头（QCarmera）或者（Player）中捕获视频帧。
 * 注意：android后景摄像头捕获的视频翻转-90度，前景摄像头翻转90度。
 * 用法：
 *      QCamera m_Camera;
 *      m_Camera.setCaptureMode(QCamera::CaptureVideo);
 *      CCaptureVideoFrame videoFrame;
 *      videoFrame.setSource(&m_Camera);
 * 注册SLOT：
 *      connect(&videoFrame, SIGNAL(CaptureFrame(const QVideoFrame&)),
 *           SLOT(CaptureVideoFrame(const QVideoFrame&)));
 * 在SLOT 中 CaptureVideoFrame(const QVideoFrame&) 处理捕获到的视频帧。
 *
 * 示例代码：
 *   QList<QByteArray> device = QCamera::availableDevices();
 *   QList<QByteArray>::iterator it;
 *   for(it = device.begin(); it != device.end(); it++)
 *   {
 *       qDebug("Camera:%s", qPrintable(QCamera::deviceDescription(*it)));
 *   }
 *
 *   QCamera camera(QCamera::availableDevices().at(1));
 *   camera.setCaptureMode(QCamera::CaptureVideo);
 *   CFrmPlayer player;
 *   CCaptureVideoFrame captureVideoFrame;
 *   if(captureVideoFrame.setSource(&camera))
 *   {
 *       qDebug("probe.setSource is ok");
 *       player.connect(&captureVideoFrame, SIGNAL(CaptureFrame(QVideoFrame)),
 *                      SLOT(present(QVideoFrame)));
 *   }
 *
 *   player.show();
 *   player.activateWindow();
 *   camera.start();
 */

#ifndef CAPTUREVIDEOFRAME_H
#define CAPTUREVIDEOFRAME_H

#include <QAbstractVideoSurface>
#include <QVideoProbe>
#include <QCamera>
#include <QThread>
#include "CaptureFrameProcess.h"

class CCaptureVideoFrame : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit CCaptureVideoFrame(QObject *parent = 0);
    virtual ~CCaptureVideoFrame();

    virtual QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

    bool setSource(QCamera *pCamera);

signals:
    //不同平台处理过后的视频帧捕获信号
    void sigCaptureFrame(const QVideoFrame &frame);
private:
signals:
    //从摄像头捕获的原始帧
    void sigRawCaptureFrame(const QVideoFrame &frame);

private slots:
    virtual bool present(const QVideoFrame &frame);
    void slotCaptureFrame(const QVideoFrame &frame);

private:
     QVideoProbe m_Probe;//android下,目前只能用probe捕获视频
     CCaptureFrameProcess m_CaptureFrameProcess;
     QThread m_Thread;

};

#endif // CAPTUREVIDEOFRAME_H
