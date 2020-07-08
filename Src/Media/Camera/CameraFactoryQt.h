#ifndef CCAMERAFACTORYQT_H
#define CCAMERAFACTORYQT_H

#include "CameraFactory.h"

/**
 * @defgroup RABBITIM_IMPLEMENT_CAMERA_QT 摄像头PJSIP实现模块  
 * @brief 摄像头PJSIP实现模块  
 * @ingroup RABBITIM_INTERFACE_CAMERA
 */

/**
 * @brief 摄像头qt类厂实现类  
 * @ingroup RABBITIM_IMPLEMENT_CAMERA_QT
 * @see CCameraFactory
 */
class CCameraFactoryQt : public CCameraFactory
{
public:
    /**
     * @brief 枚举系统中的摄像头设备  
     * @param info:摄像头设备信息列表  
     * @return 成功返回0;否则返回非0
     */
    virtual int EnumDevice(std::vector<CCameraInfo::CamerInfo> &info);
    
private:
    CCameraFactoryQt();
    virtual ~CCameraFactoryQt();
    friend class CCameraFactory;
    
    virtual CCamera* NewCamera(int nIndex);
};

#endif // CCAMERAFACTORYQT_H
