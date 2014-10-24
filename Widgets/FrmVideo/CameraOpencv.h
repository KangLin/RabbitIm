#ifndef CAMERAOPENCV_H
#define CAMERAOPENCV_H

#include <QObject>
#include <QTimer>
#include "Camera.h"
#include "opencv2/opencv.hpp"
#include "FrameProcess.h"

class CCameraOpencv : public CCamera
{
    Q_OBJECT
public:
    explicit CCameraOpencv(QObject *parent = 0);
    virtual ~CCameraOpencv();

    virtual int Start();
    virtual int Stop();

    virtual QList<QString> GetAvailableDevices();
    virtual int SetDefaultCamera();
    virtual int SetDeviceIndex(int index);
    virtual int GetDeviceIndex();
    virtual int GetOrientation();//得摄像头安装的方向,返回角度  

signals:
    void sigCaptureRawFrame(const QVideoFrame &frame);

private slots:
    void slotTimeOut();

private:
    cv::VideoCapture m_videoCapture;
    int m_deviceIndex;
    QTimer m_Timer;
    int m_tmCapture;
    CFrameProcess m_FrameProcess;
};

#endif // CAMERAOPENCV_H
