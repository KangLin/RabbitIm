#ifndef CVIDEOFRAME_H
#define CVIDEOFRAME_H

#pragma once
#include "VideoType.h"
#include "Global/Global.h"

/**
 * @brief The CVideoFrame class
 * @ingroup RABBITIM_INTERFACE RABBITIM_INTERFACE_CAMERA
 */
class RABBITIM_SHARED_LIBRARY CVideoFrame
{
public:
    CVideoFrame(const void* pData, const long &nLen, 
                const VideoInfo &videoInfo, const long &timestamp);
    CVideoFrame();
    virtual ~CVideoFrame();
    const void* GetData();
    long GetLength();
    VideoInfo m_VideoInfo; //视频信息  
    long m_Timestamp;      //时间戳  

private:
    void* m_pData;         //帧数据指针  
    long m_nLen;            //帧数据长度  
    
};

#endif // CVIDEOFRAME_H
