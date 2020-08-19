/*
 * 作者：康林 (email: kl222@126.com)
 *
 * 从摄像头（QCarmera）或者（Player）中捕获视频帧。
 * 注意：android后景摄像头捕获的视频翻转-90度，前景摄像头翻转90度。
 * 用法：
 *      QCamera m_Camera;
 *      m_Camera.setCaptureMode(QCamera::CaptureVideo);
 *      CCaptureVideoFrame videoFrame;
 *      m_Camera.setViewfinder(&videoFrame);
 * 注册SLOT：
 *      connect(&videoFrame, SIGNAL(sigCaptureFrame(const QImage&)),
 *           SLOT(CaptureVideoFrame(const QImage&)));
 * 在SLOT 中 CaptureVideoFrame(const QImage&) 处理捕获到的视频帧。
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
 *   CFrmDisplay player;
 *   CCaptureVideoFrame captureVideoFrame;
 *   m_Camera.setViewfinder(&captureVideoFrame);
 *   player.connect(&captureVideoFrame, SIGNAL(sigCaptureFrame(const QImage&)),
 *                      SLOT(slotDisplay(const QImage&)));
 *
 *   player.show();
 *   player.activateWindow();
 *   camera.start();
 */

#ifndef CAPTUREVIDEOFRAME_H_KL_2020_08_18
#define CAPTUREVIDEOFRAME_H_KL_2020_08_18

#pragma once

#include <QAbstractVideoSurface>
#ifdef ANDROID
#include <QVideoProbe>
#endif
#include <QCamera>
#include <QImage>
#include "../../Global/Global.h"

/**
 * @brief The CCameraQtCaptureVideoFrame class
 * @ingroup RABBITIM_IMPLEMENT_CAMERA_QT
 */
class RABBITIM_SHARED_LIBRARY CCameraQtCaptureVideoFrame
        : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    explicit CCameraQtCaptureVideoFrame(QObject *parent = nullptr);
    virtual ~CCameraQtCaptureVideoFrame();

    //设置支持的捕获格式  
    virtual QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    //bool isFormatSupported(const QVideoSurfaceFormat &format) const;

    int SetCameraAngle(int angle);
    
Q_SIGNALS:
    //从摄像头捕获的原始帧  
    void sigCaptureFrame(const QVideoFrame &frame);
    void sigCaptureFrame(const QImage &frame);

private Q_SLOTS:
    virtual bool present(const QVideoFrame &frame);

private:    
     int m_Angle;
};

#endif // CAPTUREVIDEOFRAME_H_KL_2020_08_18
