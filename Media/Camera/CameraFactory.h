#pragma once
#include "Camera.h"
#include <map>

/**
 * @defgroup RABBITIM_INTERFACE_CAMERA 摄像头接口类模块  
 * @brief 摄像头接口类模块  
 */

/**
 * @brief 摄像头类厂接口类  
 * @ingroup RABBITIM_INTERFACE RABBITIM_INTERFACE_CAMERA
 * 
 * @see main.cpp
 */
class CCameraFactory
{
public:
    /**
     * @brief 实例化类厂对像  
     * @return 
     */
    static CCameraFactory* Instance();
    /**
     * @brief 得到指定的摄像头  
     * @param nIndex ： 摄像头索引号  
     * @return 成功返回摄像头对像指针；否则返回 NULL  
     */
    CCamera * GetCamera(int nIndex);

    /**
     * @brief 枚举系统中的摄像头设备  
     * @param info:摄像头设备信息列表  
     * @return 成功返回0;否则返回非0  
     */
    virtual int EnumDevice(std::vector<CCameraInfo::CamerInfo> &info) = 0;

protected:
    CCameraFactory();
    virtual ~CCameraFactory();
    /**
     * @brief 实例化指定摄像头  
     * @param nIndex 摄像头索引号  
     * @return 成功返回CCamera指针，失败返回NULL  
     */
    virtual CCamera* NewCamera(int nIndex) = 0;

protected:
    std::map<int, CCamera*> m_Camera;
};

