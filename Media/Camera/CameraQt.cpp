#include "CameraQt.h"
#include "CameraFactory.h"

CCameraQt::CCameraQt(int nIndex)
    : CCamera(nIndex)
{
    m_Camera = NULL;
    m_CameraImageCapture = NULL;
}

CCameraQt::~CCameraQt()
{
}

bool CCameraQt::Present(const QVideoFrame &frame)
{
    if (m_pHander)
    {
        m_pHander->OnFrame(frame);
    }
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

    if(m_Camera)
    {
        LOG_MODEL_WARNING("CCameraQt", "Camera is opened");
        return 0;
    }

    try{
        m_Camera = 
        #if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
                new QCamera((QCamera::Position) m_nIndex);
        #else
                new QCamera(QCamera::availableDevices().at(m_nIndex));
        #endif
    }catch(...){
        LOG_MODEL_ERROR("CCameraQt", "new QCamera exception ");
    }

    if(NULL == m_Camera)
    {
        LOG_MODEL_ERROR("CCameraQt", "Don't open camera:%d", m_nIndex);
        return -1;
    }

    if (pVideoInfo)
    {
        m_VideoInfo = *pVideoInfo;
    }

    m_CameraImageCapture = new QCameraImageCapture(m_Camera);
    if(m_CameraImageCapture)
    {
        bool check = this->connect(m_CameraImageCapture,
                             SIGNAL(imageSaved(int,QString)),
                             SLOT(imageSaved(int,QString)));
        Q_ASSERT(check);
    }
    return 0;
}

int CCameraQt::OnClose()
{
    if(m_CameraImageCapture)
    {
        this->disconnect(m_CameraImageCapture);
        delete m_CameraImageCapture;
        m_CameraImageCapture = NULL;
    }
    if(m_Camera)
    {
        delete m_Camera;
        m_Camera = NULL;
    }
    return 0;
}

int CCameraQt::Start()
{
    if(NULL == m_Camera)
    {
        LOG_MODEL_ERROR("CCameraQt", "Camera don't open");
        return -1;
    }

    if(m_Camera->state() == QCamera::ActiveState)
    {
        LOG_MODEL_WARNING("CCameraQt", "Camera had started");
        return 0;
    }
    m_Camera->setCaptureMode(QCamera::CaptureVideo);
    m_CaptureFrame.setSource(this);
    m_Camera->start();
    return 0;
}

int CCameraQt::Stop()
{
    if(NULL == m_Camera)
    {
        LOG_MODEL_ERROR("CCameraQt", "Camera don't open");
        return -1;
    }

    if(m_Camera->state() == QCamera::ActiveState)
        m_Camera->stop();

    return 0;
}

int CCameraQt::Capture(const std::string &szFileName)
{
    if(NULL == m_Camera)
    {
        LOG_MODEL_ERROR("CCameraQt", "camera don't open");
        return -1;
    }

    if(QCamera::ActiveStatus != m_Camera->status())
    {
        Start();
    }
    m_Camera->setCaptureMode(QCamera::CaptureStillImage);
    m_Camera->searchAndLock();
    m_CameraImageCaptureID = m_CameraImageCapture->capture(QString(szFileName.c_str()));
    m_Camera->unlock();
    m_Camera->setCaptureMode(QCamera::CaptureVideo);
    return 0;
}

void CCameraQt::imageSaved(int id, const QString &fileName)
{
    if(m_CameraImageCaptureID != id)
        return;
    if (m_pHander)
    {
        int nRet = 0;
        nRet = m_pHander->OnCapture(fileName.toStdString());
    }
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
