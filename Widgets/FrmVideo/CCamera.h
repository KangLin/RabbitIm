#ifndef CCAMERA_H
#define CCAMERA_H

#include <QByteArray>
#include <QList>
#include <QCamera>
#include <QAbstractVideoBuffer>
#include <QVideoFrame>
#include "CaptureVideoFrame.h"
#include "FrameProcess.h"

#include <QObject>

class CCamera : public QObject
{
    Q_OBJECT
public:
    explicit CCamera(QObject *parent = 0);
    virtual ~CCamera();

	int Start();
    int Stop();
    QList<QString> GetAvailableDevices();
#ifdef ANDROID
    QCamera::Position GetCameraPoistion();
#endif
    int SetDeviceIndex(int index);
    int GetDeviceIndex();

signals:
    //不同平台处理过后的视频帧捕获信号  
    void sigCaptureFrame(const QVideoFrame &frame);
    //转换视频帧格式后触发的信号，用于视频压缩发送  
    void sigConvertedToYUYVFrame(const QXmppVideoFrame &frame);

private:
    //摄像头位置  
    QByteArray m_CameraPosition;
    QCamera *m_pCamera;

    CCaptureVideoFrame m_CaptureVideoFrame;//实现捕获视频帧
    CFrameProcess m_CaptureFrameProcess;
    
private slots:
    void updateCameraState(QCamera::State state);
    void displayCameraError(QCamera::Error e);
};

#endif // CCAMERA_H
