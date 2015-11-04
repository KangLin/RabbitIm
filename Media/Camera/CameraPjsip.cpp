#include "CameraPjsip.h"
#include <assert.h>

CCameraPjsip::CCameraPjsip(int nIndex) : CCamera(nIndex)
{
    m_VidDevCb.capture_cb = capture_cb;
    m_pVidDevStream = NULL;
    Init();
}

CCameraPjsip::~CCameraPjsip()
{
    Clean();
}

int CCameraPjsip::Init()
{
    pj_caching_pool_init(&m_Caching_pool, &pj_pool_factory_default_policy, 0);
    return 0;
}

int CCameraPjsip::Clean()
{
    pj_caching_pool_destroy(&m_Caching_pool);
    return 0;
}

int CCameraPjsip::OnOpen(VideoInfo* pVideoInfo)
{
    int nRet = 0;
    pj_status_t status = PJ_FALSE;
    
    if (m_pVidDevStream)
    {
        LOG_MODEL_ERROR("CCamera", "camera has opened");
        return -1;
    }

    pj_caching_pool_init(&m_Caching_pool, &pj_pool_factory_default_policy, 0);
    pj_pool_t* pPool = pj_pool_create(&m_Caching_pool.factory,
        "carmeraPara", 1000, 1000, NULL);
    if (NULL == pPool)
    {
        LOG_MODEL_ERROR("CCamera", "pj_pool_create fail");
        return -2;
    }
    pjmedia_vid_dev_param param;
    status = pjmedia_vid_dev_default_param(pPool, m_nIndex, &param);
    if (PJ_SUCCESS != status)
    {
        LOG_MODEL_ERROR("CCamera", "pjmedia_vid_dev_default_param fail");
        pj_pool_release(pPool);
        return -3;
    }
    pj_pool_release(pPool);

    if (param.fmt.detail_type != PJMEDIA_FORMAT_DETAIL_VIDEO) {
        LOG_MODEL_ERROR("CCamera", "deivce isn't video capture");
        return -4;
    }

    if (pVideoInfo)
    {
        pjmedia_format fmt;
        pjmedia_format_init_video(&fmt, pVideoInfo->Format,
            pVideoInfo->nWidth,
            pVideoInfo->nHeight,
            pVideoInfo->nRatio,
            1
            );
        param.fmt = fmt;
        m_VideoInfo = *pVideoInfo;
    }
    param.dir = PJMEDIA_DIR_CAPTURE;
    
    status = pjmedia_vid_dev_stream_create(&param, &m_VidDevCb,
        this, &m_pVidDevStream);
    if (PJ_SUCCESS != status)
    {
        LOG_MODEL_ERROR("CCamera", "pjmedia_vid_dev_stream_create faile");
        return -5;
    }
    //检查设备指定的格式  
    pjmedia_video_format_detail *vfd;
    vfd = pjmedia_format_get_video_format_detail(&param.fmt, PJ_FALSE);
    if (vfd == NULL)
    {
        LOG_MODEL_ERROR("CCamera", "deivce don't contains the format");
        Close();
        return -6;
    }
    m_VideoInfo.nWidth = vfd->size.w;
    m_VideoInfo.nHeight = vfd->size.h;
    m_VideoInfo.nRatio = vfd->fps.num;
    m_VideoInfo.Format = (VideoFormat) param.fmt.id;

    return nRet;
}

int CCameraPjsip::OnClose()
{
    int nRet = 0;
    pj_status_t status = PJ_FALSE;
    if (m_pVidDevStream)
    {
        status = pjmedia_vid_dev_stream_destroy(m_pVidDevStream);
        if (PJ_SUCCESS == status)
            m_pVidDevStream = NULL;
        else
        {
            LOG_MODEL_ERROR("CCamera", "Close faile");
            nRet = -1;
        }
    }
    
    return nRet;
}

int CCameraPjsip::Start()
{
    int nRet = 0;
    pj_status_t status = PJ_FALSE;
    if (m_pVidDevStream)
    {
        status = pjmedia_vid_dev_stream_start(m_pVidDevStream);
        if (PJ_SUCCESS != status)
        {
            LOG_MODEL_ERROR("CCamera", "pjmedia_vid_dev_stream_start faile");
            return -1;
        }
    }
    return nRet;
}

int CCameraPjsip::Stop()
{
    int nRet = 0;
    pj_status_t status = PJ_FALSE;
    if (m_pVidDevStream)
    {
        status = pjmedia_vid_dev_stream_stop(m_pVidDevStream);
        if (PJ_SUCCESS != status)
        {
            LOG_MODEL_ERROR("CCamera", "pjmedia_vid_dev_stream_stop faile");
            return -1;
        }
    }
    return nRet;
}

int CCameraPjsip::OnFrame(pjmedia_frame *frame)
{
    assert(PJMEDIA_FRAME_TYPE_VIDEO == frame->type);
    std::shared_ptr<CVideoFrame> f(new CVideoFrame(frame->buf, 
          frame->size, m_VideoInfo, (long)frame->timestamp.u64));
    if (m_pHander)
        return m_pHander->OnFrame(f);
    return 0;
}

pj_status_t CCameraPjsip::capture_cb(pjmedia_vid_dev_stream *stream,
    void *user_data, pjmedia_frame *frame)
{
    int nRet = 0;
    CCameraPjsip *pThis = (CCameraPjsip*)user_data;
    assert(pThis);
    nRet = pThis->OnFrame(frame);
    if (nRet)
        return PJ_FALSE;

    return PJ_SUCCESS;
}
