#pragma once
#include "CameraInfo.h"
#include "VideoFrame.h"
#include <memory>

/**
 * @brief 摄像头抽像类  
 *    CCamera::CHanderFrame Hander;
 *    CCamera* pCamera = CCameraFactory::Instance()->GetCamera(0) //得到对象  
 *    pCamera->Open(&Hander);
 *    pCamera->Start();
 *    ........
 *    pCamera->Stop();
 *    pCamera->Close();
 * @ingroup RABBITIM_INTERFACE RABBITIM_INTERFACE_CAMERA
 * @see CCameraFactory
 */
class CCamera
{
public:
    CCamera(int nIndex);
    virtual ~CCamera();

    /**
     * @brief 捕获视频帧处理类  
     * @ingroup RABBITIM_INTERFACE RABBITIM_INTERFACE_CAMERA
     */
    class CHanderFrame{
    public:
        /**
         * @brief 重载此函数进行帧处理  
         * @param frame 捕获的帧  
         * @return 
         */
        virtual int OnFrame(const std::shared_ptr<CVideoFrame> frame);
    };

    /**
     * @brief 得到捕获头信息  
     * @return 
     */
    CCameraInfo* GetCameraInfo();
    /**
     * @brief 得到捕获视频信息  
     * @return 
     */
    virtual VideoInfo GetCurrentVideoInfo();
    virtual int SetCurrentVideoInfo(VideoInfo videoInfo);
    /**
     * @brief 打开摄像头设备  
     * @param pHander:视频捕获处理者  
     * @param pVideoInfo:要捕获的视频信息  
     * @return 
     */
    int Open(CHanderFrame* pHander = NULL, VideoInfo* pVideoInfo = NULL);
    
    /**
     * @brief 关闭摄像头设备  
     * @return 
     */
    virtual int Close();
    
    /**
     * @brief 开始捕获视频  
     * @return 
     */
    virtual int Start() = 0;
    /**
     * @brief 停止捕获视频  
     * @return 
     */
    virtual int Stop() = 0;

private:
    virtual int OnOpen(VideoInfo *pVideoInfo) = 0;
    virtual int OnClose() = 0;
    
protected:
    CHanderFrame* m_pHander; ///< 处理者指针  
    VideoInfo m_VideoInfo;  ///< 视频格式  
    int m_nIndex;           ///< 设备索引号  
    
private:
    CCameraInfo* m_pCameraInfo;
    CHanderFrame m_Hander;
};
