#include "CameraOpencv.h"
#include "Global/Global.h"
#include "DataVideoBuffer.h"
#include <QImage>

CCameraOpencv::CCameraOpencv(QObject *parent) :
    CCamera(parent)
{
    m_deviceIndex = 0;
    m_tmCapture = 1000 / 30;
    bool check = connect(&m_Timer, SIGNAL(timeout()),
                         SLOT(slotTimeOut()));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(sigCaptureRawFrame(QVideoFrame)), 
                    &m_FrameProcess, SLOT(slotCaptureFrame(QVideoFrame)));
    Q_ASSERT(check);
    check = connect(&m_FrameProcess, SIGNAL(sigCaptureFrame(QVideoFrame)),
                    this, SIGNAL(sigCaptureFrame(QVideoFrame)));
    Q_ASSERT(check);
}

CCameraOpencv::~CCameraOpencv()
{}

int CCameraOpencv::Start()
{
    int nRet = 0;
    if(!m_videoCapture.open(m_deviceIndex))
    {
        LOG_MODEL_DEBUG("CCameraOpencv", "don't open video deivce:%d", m_deviceIndex);
        return -1;
    }

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
    cv::Mat frame;
#ifdef ANDROID
    if(!m_videoCapture.grab())
        return;
     m_videoCapture.retrieve(frame, cv::CAP_ANDROID_COLOR_FRAME_RGB);
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

    //*第一种转换方法：用QImage  
    QImage image((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);  //RGB888就是RGB24即RGB  
    QVideoFrame outFrame(image);//*/

    /*第二种转换方法：用CDataVideoBuffer  
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
    
    emit sigCaptureRawFrame(outFrame);
}

QList<QString> CCameraOpencv::GetAvailableDevices()
{
    QList<QString> ret;
    
    return ret;
}

int CCameraOpencv::SetDefaultCamera()
{
    int nRet = 0;
    
    return nRet;
}

int CCameraOpencv::SetDeviceIndex(int index)
{
    int nRet = 0;
    
    return nRet;
}

int CCameraOpencv::GetDeviceIndex()
{
    int nRet = 0;
    
    return nRet;
}

int CCameraOpencv::GetOrientation()
{
    int nRet = 0;
    
    return nRet;
}
