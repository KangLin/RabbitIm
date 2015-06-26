#pragma once
#include "CameraFactory.h"
#include <pjmedia.h>
#include <pjlib.h>

/**
 * @defgroup RABBITIM_IMPLEMENT_CAMERA_PJSIP 摄像头PJSIP实现模块  
 * @brief 摄像头PJSIP实现模块  
 * @ingroup RABBITIM_INTERFACE_CAMERA
 */

/**
 * @brief 摄像头类厂pjsip实现类  
 * @ingroup RABBITIM_IMPLEMENT_CAMERA_PJSIP
 * @see CCameraFactory
 */
class CCameraFactoryPjsip : public CCameraFactory
{
public:
    /**
     * @brief 枚举系统中的摄像头设备  
     * @param info:摄像头设备信息列表  
     * @return 成功返回0;否则返回非0
     */
    virtual int EnumDevice(std::vector<CCameraInfo::CamerInfo> &info);

private:
    CCameraFactoryPjsip();
    virtual ~CCameraFactoryPjsip();
    friend class CCameraFactory;

    int Init();
    int Clean();
    virtual CCamera* NewCamera(int nIndex);

    pj_caching_pool m_Caching_pool;
    pj_pool_t *m_pPool;
    
    static void log_func(int level, const char *data, int len);
};

