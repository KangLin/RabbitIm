#include "CameraInfo.h"
#include "CameraFactory.h"

CCameraInfo::CCameraInfo(int nIndex)
{
    m_nIndex = nIndex;
    m_pCameraInfo = NULL;
}

CCameraInfo::~CCameraInfo()
{
    if(m_pCameraInfo)
        delete m_pCameraInfo;
}

CCameraInfo::CamerInfo CCameraInfo::GetCameraInfo()
{
    if (m_pCameraInfo)
        return *m_pCameraInfo;

    CCameraInfo::CamerInfo camerInfo;
    std::vector<CCameraInfo::CamerInfo> info;
    CCameraFactory::Instance()->EnumDevice(info);
    if (m_nIndex < 0 || m_nIndex >= info.size())
    {
        return camerInfo;
    }
    camerInfo = info[m_nIndex];
    m_pCameraInfo = new CCameraInfo::CamerInfo();
    if (m_pCameraInfo)
        *m_pCameraInfo = camerInfo;

    return camerInfo;
}
