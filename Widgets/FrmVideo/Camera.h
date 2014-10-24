#ifndef CCAMERA_H
#define CCAMERA_H

#include <QByteArray>
#include <QList>
#include <QCamera>
#include <QAbstractVideoBuffer>
#include <QVideoFrame>
#include <QObject>
#include "CaptureVideoFrame.h"
#include "FrameProcess.h"

class CCamera : public QObject
{
    Q_OBJECT
public:
    explicit CCamera(QObject *parent = 0);
    virtual ~CCamera();

    virtual int Start();
    virtual int Stop();

    virtual QList<QString> GetAvailableDevices();
#ifdef MOBILE
    enum Position{
        UnspecifiedPosition,
        BackFace,
        FrontFace
    };
    virtual Position GetCameraPoistion();
#endif
    virtual int SetDefaultCamera();
    virtual int SetDeviceIndex(int index);
    virtual int GetDeviceIndex();
    virtual int GetOrientation();//得摄像头安装的方向,返回角度  

signals:
    //不同平台处理过后的视频帧捕获信号  
    void sigCaptureFrame(const QVideoFrame &frame);

private:
    //摄像头位置  
    QByteArray m_CameraPosition;
    QCamera *m_pCamera;

    CCaptureVideoFrame m_CaptureVideoFrame;//实现捕获视频帧  
protected:
    CFrameProcess m_CaptureFrameProcess;

private slots:
    void updateCameraState(QCamera::State state);
    void displayCameraError(QCamera::Error e);
};

#endif // CCAMERA_H
