#include "CameraQt.h"
#include "CameraFactory.h"
#include <QCameraInfo>
#include <QMatrix>

CCameraQt::CCameraQt(int nIndex)
    : CCamera(nIndex)
{
    m_pCamera = NULL;
    m_pCameraImageCapture = NULL;
}

CCameraQt::~CCameraQt()
{
}

bool CCameraQt::Present(const QVideoFrame &frame)
{
    
    QVideoFrame outFrame;
    
    //增加方向校正，屏幕纵座标是向下的  
    //
    //   (0,0)  --------------------------------->  X 轴
    //    |  |----------------------------------------|
    //    |  |                                        |
    //    |  |                                        |
    //    |  |                                        |
    //    |  |                                        |
    //    |  |               Screen                   |
    //    |  |                                        |
    //    |  |                                        |
    //    |  |                                        |
    //    |  |                                        |
    //    |  |                                        |
    //    |  |----------------------------------------|
    //    Y
    // Y轴 
    
    double nAngle = 0;
    QList<QCameraInfo> cameraInfos = QCameraInfo::availableCameras();
    QCameraInfo info = cameraInfos.at(m_nIndex);
    LOG_MODEL_DEBUG("CCamerQT", "orientation=%d; format:%d", info.orientation(), frame.pixelFormat());
#ifdef MOBILE
    nAngle = 360 - info.orientation();
#else
    switch(info.orientation()) //摄像头传感器相对于屏幕坐标的角度（顺时针）  
    {
    case 0: //需要翻转  
        nAngle = 180;
        break;
    case 90:  //逆时针转 90  
        nAngle = 270;
        break;
    case 180: //不用操作  
        nAngle = 0;
        break;
    case 270: //顺时针转 90 
        nAngle = 90;
        
        break;
    default:
        LOG_MODEL_WARNING("CCamerQt",
                          "The orientation[%d] don't support.",
                          info.orientation());
    }
#endif
    int nRet = CTool::ImageRotate(frame, outFrame, nAngle);
    if(nRet)
        outFrame = frame;
    emit sigCaptureFrame(outFrame);
    if (m_pHander)
        m_pHander->OnFrame(outFrame);
    
    return true;
}

int CCameraQt::OnOpen(VideoInfo *pVideoInfo)
{
    std::vector<CCameraInfo::CamerInfo> info;
    CCameraFactory::Instance()->EnumDevice(info);
    if(0 < m_nIndex || m_nIndex >  info.size() - 1)
    {
        LOG_MODEL_ERROR("CCameraQt", "Camera [%d] is invalid", m_nIndex);
        return -1;
    }

    if(m_pCamera)
    {
        LOG_MODEL_WARNING("CCameraQt", "Camera is opened");
        return 0;
    }

    try{
        m_pCamera =
        #if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
                new QCamera((QCamera::Position) m_nIndex);
        #else
                new QCamera(QCamera::availableDevices().at(m_nIndex));
        #endif
    }catch(...){
        LOG_MODEL_ERROR("CCameraQt", "new QCamera exception ");
    }

    if(NULL == m_pCamera)
    {
        LOG_MODEL_ERROR("CCameraQt", "Don't open camera:%d", m_nIndex);
        return -1;
    }

    if (pVideoInfo)
    {
        m_VideoInfo = *pVideoInfo;
    }

    m_pCameraImageCapture = new QCameraImageCapture(m_pCamera);
    if(m_pCameraImageCapture)
    {
        bool check = this->connect(m_pCameraImageCapture,
                             SIGNAL(imageSaved(int,QString)),
                             SLOT(slotImageSaved(int,QString)));
        Q_ASSERT(check);
    }
    return 0;
}

int CCameraQt::OnClose()
{
    if(m_pCameraImageCapture)
    {
        this->disconnect(m_pCameraImageCapture);
        delete m_pCameraImageCapture;
        m_pCameraImageCapture = NULL;
    }
    if(m_pCamera)
    {
        delete m_pCamera;
        m_pCamera = NULL;
    }
    return 0;
}

int CCameraQt::Start()
{
    if(NULL == m_pCamera)
    {
        LOG_MODEL_ERROR("CCameraQt", "Camera don't open");
        return -1;
    }

    if(m_pCamera->state() == QCamera::ActiveState)
    {
        LOG_MODEL_WARNING("CCameraQt", "Camera had started");
        return 0;
    }
    m_pCamera->setCaptureMode(QCamera::CaptureVideo);
    m_CaptureFrame.setSource(this);
    m_pCamera->start();
    return 0;
}

int CCameraQt::Stop()
{
    if(NULL == m_pCamera)
    {
        LOG_MODEL_ERROR("CCameraQt", "Camera don't open");
        return -1;
    }

    if(m_pCamera->state() == QCamera::ActiveState)
        m_pCamera->stop();

    return 0;
}

int CCameraQt::Capture(const std::string &szFileName)
{
    if(NULL == m_pCamera)
    {
        LOG_MODEL_ERROR("CCameraQt", "camera don't open");
        return -1;
    }

    if(QCamera::ActiveStatus != m_pCamera->status())
    {
        Start();
    }
    m_pCamera->setCaptureMode(QCamera::CaptureStillImage);
    m_pCamera->searchAndLock();
    m_CameraImageCaptureID = m_pCameraImageCapture->capture(QString(szFileName.c_str()));
    m_pCamera->unlock();
    m_pCamera->setCaptureMode(QCamera::CaptureVideo);
    return 0;
}

void CCameraQt::slotImageSaved(int id, const QString &fileName)
{
    if(m_CameraImageCaptureID != id)
        return;
    if (m_pHander)
    {
        int nRet = 0;
        nRet = m_pHander->OnCapture(fileName.toStdString());
    }
    emit sigCapturePicture(fileName);
}

VideoFormat CCameraQt::QVideoFrameFormatToVideoFormat(const QVideoFrame::PixelFormat format)
{
    if(QVideoFrame::Format_RGB32 == format)
        return VIDEO_FORMAT_RGB32;
    else if(QVideoFrame::Format_BGR24 == format)
        return VIDEO_FORMAT_BGRA;
    else if(QVideoFrame::Format_RGB24 == format)
        return VIDEO_FORMAT_RGB24;
    else if(QVideoFrame::Format_UYVY == format)
        return VIDEO_FORMAT_UYVY;
    else if(QVideoFrame::Format_NV21 == format)
        return VIDEO_FORMAT_NV21;
    return VIDEO_FORMAT_NONE;
}
