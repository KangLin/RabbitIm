#ifndef CAMERAOPENCV_H
#define CAMERAOPENCV_H

#include <QObject>
#include <QTimer>
#include "CameraQt.h"
#include "opencv2/opencv.hpp"
#include "FrameProcess.h"

class CCameraOpencv : public CCameraQt
{
    Q_OBJECT
public:
    explicit CCameraOpencv(QObject *parent = 0);
    virtual ~CCameraOpencv();

    virtual int Start();
    virtual int Stop();

signals:
    void sigCaptureRawFrame(const QVideoFrame &frame);

private slots:
    void slotTimeOut();

private:
    cv::VideoCapture m_videoCapture;
    QTimer m_Timer;
    int m_tmCapture;
};

#endif // CAMERAOPENCV_H
