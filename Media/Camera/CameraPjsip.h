#pragma once
#include "Camera.h"
#include <pjmedia.h>
#include <pjlib.h>
#include "VideoFrame.h"

/**
 * @brief 摄像头 pjsip 实现类  
 * @ingroup RABBITIM_IMPLEMENT_CAMERA_PJSIP
 * @see CCamera
 */
class CCameraPjsip : public CCamera
{
public:
    CCameraPjsip(int nIndex);
    virtual ~CCameraPjsip();

    virtual int Start();
    virtual int Stop();
    
    virtual int OnFrame(pjmedia_frame *frame);

private:
    virtual int OnOpen(VideoInfo* pVideoInfo = NULL);
    virtual int OnClose();
    
private:
    pj_caching_pool m_Caching_pool;
    pjmedia_vid_dev_stream * m_pVidDevStream;
    pjmedia_vid_dev_cb m_VidDevCb;
    
    static pj_status_t capture_cb(pjmedia_vid_dev_stream *stream, void *user_data, pjmedia_frame *frame);
    int Init();
    int Clean();
};

