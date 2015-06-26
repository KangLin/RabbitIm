#include "Camera.h"

CCamera::CCamera(int nIndex)
{
    m_pHander = NULL;
    m_nIndex = nIndex;
    m_pCameraInfo = NULL;
    m_VideoInfo.Format = VIDEO_FORMAT_NONE;
    m_VideoInfo.nRatio = 15;
}

CCamera::~CCamera()
{
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
