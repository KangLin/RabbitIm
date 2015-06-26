#pragma once

#include <string>
#include <vector>
#include "VideoType.h"

/**
 * @brief 摄像头信息类接口类\n  
 *        通过 CCamera::GetCameraInfo() 得到其实例  
 * @ingroup RABBITIM_INTERFACE RABBITIM_INTERFACE_CAMERA
 */
class CCameraInfo
{
public:
    CCameraInfo(int nIndex = 0);
    virtual ~CCameraInfo();
    
    struct CamerInfo{
        int nIndex;
        std::string szName;
        std::string szDescription;
        std::vector<VideoInfo> Format;
    };

    /**
     * @brief 得到当前摄像头设备信息  
     * @return 
     */
    virtual CamerInfo GetCameraInfo();
    
protected:
    int m_nIndex;

private:
    CamerInfo* m_pCameraInfo;
};
