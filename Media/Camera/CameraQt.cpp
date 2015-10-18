#include "CameraQt.h"

CCameraQt::CCameraQt(int nIndex)
    : CCamera(nIndex)
{
}

CCameraQt::~CCameraQt()
{
}

bool CCameraQt::Present(const QVideoFrame &frame)
{
    QVideoFrame f(frame);
    if(!f.map(QAbstractVideoBuffer::ReadOnly))
        return false;

    m_VideoInfo.nHeight = f.height();
    m_VideoInfo.nWidth = f.width();
    m_VideoInfo.Format = QVideoFrameFormatToVideoFormat(
                f.pixelFormat());
    std::shared_ptr<CVideoFrame> vf(new CVideoFrame(f.bits(),
          f.mappedBytes(), m_VideoInfo, (long)f.startTime()));
    if (m_pHander)
    {
        int nRet = 0;
        nRet = m_pHander->OnFrame(vf);
    }
    f.unmap();
    return true;
}

int CCameraQt::Open(CHanderFrame *pHander, VideoInfo *pVideoInfo)
{
    if(NULL == m_Camera.get())
        m_Camera = std::auto_ptr<QCamera>(
            #if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
                    new QCamera((QCamera::Position) m_nIndex)
            #else
                    new QCamera(QCamera::availableDevices().at(m_nIndex))
            #endif
                    );

    m_pHander = pHander;
    if (pVideoInfo)
    {
        m_VideoInfo = *pVideoInfo;
    }
    return 0;
}

int CCameraQt::Close()
{
    if(m_Camera.get())
    {
        m_Camera.release();
    }
    return 0;
}

int CCameraQt::Start()
{
    if(NULL == m_Camera.get())
    {
        LOG_MODEL_ERROR("CCameraQt", "camera don't open");
        return -1;
    }
    m_Camera->setCaptureMode(QCamera::CaptureVideo);
    m_CaptureFrame.setSource(this);
    m_Camera->start();
    return 0;
}

int CCameraQt::Stop()
{
    if(NULL == m_Camera.get())
    {
        LOG_MODEL_ERROR("CCameraQt", "camera don't open");
        return -1;
    }

    m_Camera->stop();

    return 0;
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
