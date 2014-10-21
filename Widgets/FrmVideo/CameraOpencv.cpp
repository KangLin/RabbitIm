#include "CameraOpencv.h"
#include "Global/Global.h"
#include "DataVideoBuffer.h"

CCameraOpencv::CCameraOpencv(QObject *parent) :
    CCamera(parent)
{
    m_deviceIndex = 0;
    m_tmCapture = 1000 / 30;
    bool check = connect(&m_Timer, SIGNAL(timeout()),
                         SLOT(slotTimeOut()));
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
    m_videoCapture >> frame;
    QByteArray outData;
    //outData.resize(frame.total() * frame.channels());//frame.total指图片像素个数，总字节数(dst.data)=dst.total*dst.channels() 
    //outData.append(frame.data, frame.total() * frame.channels());
    //由QVideoFrame进行释放  
    CDataVideoBuffer* pBuffer = new CDataVideoBuffer(outData,
                            frame.cols,
                            frame.rows);
    //double dbFormat = m_videoCapture.get(cv::CAP_PROP_FORMAT);
    //LOG_MODEL_DEBUG("CCameraOpencv", "CCameraOpencv format:%d", dbFormat);
    QVideoFrame outFrame(pBuffer,
                         QSize(frame.cols,
                               frame.rows),
                         QVideoFrame::Format_RGB32);//*/
    
    emit sigCaptureFrame(outFrame);
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
