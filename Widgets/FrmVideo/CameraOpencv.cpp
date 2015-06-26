#include "CameraOpencv.h"
#include "Global/Global.h"
#include "DataVideoBuffer.h"
#include <QImage>

CCameraOpencv::CCameraOpencv(QObject *parent) :
    CCameraQt(parent)
{
    m_tmCapture = 1000 / GetFrameRate();
    bool check = connect(&m_Timer, SIGNAL(timeout()),
                         SLOT(slotTimeOut()));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigCaptureRawFrame(QVideoFrame)), 
                    &m_CaptureFrameProcess, SLOT(slotCaptureFrame(QVideoFrame)));
    Q_ASSERT(check);
}

CCameraOpencv::~CCameraOpencv()
{}

int CCameraOpencv::Start()
{
    int nRet = 0;
    if(m_videoCapture.isOpened())
        Stop();
    if(!m_videoCapture.open(this->GetDeviceIndex()))
    {
        LOG_MODEL_DEBUG("CCameraOpencv", "don't open video deivce:%d", GetDeviceIndex());
        return -1;
    }

    m_nFrameWidth = m_videoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
    m_nFrameHeight = m_videoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
    //m_nFrameWidth = m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
    //m_nFrameHeight = m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
    m_Timer.start(m_tmCapture);
    return nRet;
}

int CCameraOpencv::Stop()
{
    int nRet = 0;
    m_Timer.stop();
    m_videoCapture.release();
    return nRet;
}

void CCameraOpencv::slotTimeOut()
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CCameraOpencv", "CCameraOpencv::slotTimeOut threadid:0x%X", QThread::currentThread());
#endif

    cv::Mat frame;
#ifdef ANDROID
    if(!m_videoCapture.grab())
        return;
     m_videoCapture.retrieve(frame, CV_CAP_ANDROID_COLOR_FRAME_RGB);//cv::CAP_ANDROID_COLOR_FRAME_RGB
#else
    m_videoCapture >> frame;
    //因为opencv会在内部把图像转化为BGR格式  
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);  
#endif
    if(frame.empty())
        return;
    /*LOG_MODEL_DEBUG("CCameraOpencv", "frame.type:%d;format:%d", frame.type(), 
                #ifdef ANDROID
                    m_videoCapture.get(cv::CAP_PROP_ANDROID_PREVIEW_FORMAT)
                #else
                    m_videoCapture.get(cv::CAP_PROP_FORMAT)
                #endif
                    );
    */

    /*第一种转换方法：用QImage  
    QImage image((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);  //RGB888就是RGB24即RGB  
    QVideoFrame outFrame(image);//*/

    //*第二种转换方法：用CDataVideoBuffer  
    QByteArray outData((const char*)frame.data, (int)(frame.total() * frame.channels()));
    //frame.total指图片像素个数，总字节数(dst.data)=dst.total*dst.channels()  
    //由QVideoFrame进行释放  
    CDataVideoBuffer* pBuffer = new CDataVideoBuffer(outData,
                            frame.cols,
                            frame.rows);
    //LOG_MODEL_DEBUG("CCameraOpencv", "CCameraOpencv format:%d", dbFormat);
    QVideoFrame outFrame(pBuffer,
                         QSize(frame.cols,
                               frame.rows),
                         QVideoFrame::Format_RGB24);//*/
#ifdef ANDROID
    emit sigCaptureRawFrame(outFrame);
#else
    emit sigCaptureFrame(outFrame);//opencv已经做过镜像了  
#endif
}
