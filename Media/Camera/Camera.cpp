#include "Camera.h"

CCamera::CCamera(int nIndex)
#ifdef QT_CORE_LIB
    : QObject()
#endif
{
    m_pHander = NULL;
    m_nIndex = nIndex;
    m_pCameraInfo = NULL;
    m_VideoInfo.Format = VIDEO_FORMAT_NONE;
    m_VideoInfo.nRatio = 15;
}

CCamera::~CCamera()
{
    Close();
    if(m_pCameraInfo)
        delete m_pCameraInfo;
}

VideoInfo CCamera::GetCurrentVideoInfo()
{
    return m_VideoInfo;
}

int CCamera::SetCurrentVideoInfo(VideoInfo videoInfo)
{
    m_VideoInfo = videoInfo;
    return 0;
}

CCameraInfo* CCamera::GetCameraInfo()
{
    if(NULL == m_pCameraInfo)
        m_pCameraInfo = new CCameraInfo(m_nIndex);
    return m_pCameraInfo;
}

#ifdef QT_CORE_LIB
int CCamera::CHanderFrame::OnFrame(const QVideoFrame &frame)
{
    LOG_MODEL_ERROR("CCamera", "Please implement CCamera::CHanderFrame::OnFrame");
    return 0;
}
#else
int CCamera::CHanderFrame::OnFrame(const std::shared_ptr<CVideoFrame> frame)
{
    LOG_MODEL_ERROR("CCamera", "Please implement CCamera::CHanderFrame::OnFrame");
    return 0;
}
#endif

int CCamera::CHanderFrame::OnCapture(const std::string szFile)
{
    LOG_MODEL_ERROR("CCamera", "Please implement CCamera::OnCapture::OnFrame");
    return 0;
}

int CCamera::Open(CHanderFrame *pHander, VideoInfo *pVideoInfo)
{
    m_pHander = pHander;
    if(NULL == m_pHander)
        m_pHander = &m_DefaultHander;
    return OnOpen(pVideoInfo);
}

int CCamera::Close()
{
    OnClose();
    m_pHander = NULL;
    return 0;
}
