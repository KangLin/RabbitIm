#include "CapThread.h"

CCapThread::CCapThread(QObject *parent) : QThread(parent)
{
    m_bStop = false;
}

CCapThread::~CCapThread()
{
    Stop();
}

void CCapThread::run()
{
    m_bStop = false;
    
    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        return;
 
    while(!m_bStop)
    {
        cv::Mat frame;
        cap >> frame;
        emit sigCaptured(frame);
    }
}

void CCapThread::Stop(bool bStop)
{
    m_bStop = bStop;
}
