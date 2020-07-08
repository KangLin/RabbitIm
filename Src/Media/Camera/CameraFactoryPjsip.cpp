#include "CameraFactoryPjsip.h"
#include "CameraPjsip.h"

CCameraFactoryPjsip::CCameraFactoryPjsip() : CCameraFactory()
{
    m_pPool = NULL;
    Init();
}

CCameraFactoryPjsip::~CCameraFactoryPjsip()
{
    Clean();
}

CCamera* CCameraFactoryPjsip::NewCamera(int nIndex)
{
    return new CCameraPjsip(nIndex);
}

int CCameraFactoryPjsip::EnumDevice(std::vector<CCameraInfo::CamerInfo> &info)
{
    int nRet = 0;
    int nCount = 0;
    pj_status_t status = PJ_FALSE;
    nCount = pjmedia_vid_dev_count();
    for (int i = 0; i < nCount; i++)
    {
        pjmedia_vid_dev_info devInfo;
        status = pjmedia_vid_dev_get_info(i, &devInfo);
        if (PJ_SUCCESS != status)
            continue;

        if (devInfo.fmt_cnt <= 0)
            continue;

        CCameraInfo::CamerInfo ci;
        ci.nIndex = i;
        ci.szName = devInfo.name;

        for (int c = 0; c < devInfo.fmt_cnt; c++)
        {
            if (PJMEDIA_TYPE_VIDEO != devInfo.fmt[c].type
                || PJMEDIA_FORMAT_DETAIL_VIDEO != devInfo.fmt[c].detail_type)
                continue;
            const pjmedia_video_format_info *vfi;
            vfi = pjmedia_get_video_format_info(NULL, devInfo.fmt[c].id);
            VideoInfo pixInfo;
            pixInfo.Format = (VideoFormat)devInfo.fmt[c].id;
            pixInfo.nWidth = devInfo.fmt[c].det.vid.size.w;
            pixInfo.nHeight = devInfo.fmt[c].det.vid.size.h;
            pixInfo.nRatio = devInfo.fmt[c].det.vid.fps.num;
            ci.Format.push_back(pixInfo);
        }

        info.push_back(ci);

    }
    return nRet;
}


int CCameraFactoryPjsip::Init()
{
    pj_status_t status = PJ_FALSE;
    pj_log_set_log_func(log_func);
    pj_init();
    pj_caching_pool_init(&m_Caching_pool, &pj_pool_factory_default_policy, 0);
    m_pPool = pj_pool_create(&m_Caching_pool.factory,
        "camer", 1000, 512, NULL);
    if (NULL == m_pPool)
        return -1;
#ifdef DEBUG
    pj_log_set_decor(PJ_LOG_HAS_NEWLINE);
    pj_log_set_level(3);
#endif
    status = pjmedia_video_format_mgr_create(m_pPool, 64, 0, NULL);
    if (status != PJ_SUCCESS)
        return -2;
    status = pjmedia_converter_mgr_create(m_pPool, NULL);
    if (status != PJ_SUCCESS)
        return -3;
    status = pjmedia_event_mgr_create(m_pPool, 0, NULL);
    if (status != PJ_SUCCESS)
        return -4;
    status = pjmedia_vid_codec_mgr_create(m_pPool, NULL);
    if (status != PJ_SUCCESS)
        return -5;
    //初始化视频设备子系统  
    status = pjmedia_vid_dev_subsys_init(&m_Caching_pool.factory);
    if (status != PJ_SUCCESS)
        return -6;
    return 0;
}

int CCameraFactoryPjsip::Clean()
{
    int nRet = 0;
    pj_status_t status = PJ_FALSE;
    //关闭视频设备子系统  
    status = pjmedia_vid_dev_subsys_shutdown();
    PJ_SUCCESS == status ? nRet = 0 : nRet = -1;

    pj_pool_release(m_pPool);
    m_pPool = NULL;
    pj_caching_pool_destroy(&m_Caching_pool);
    pj_shutdown();

    return nRet;
}

void CCameraFactoryPjsip::log_func(int level, const char *data, int len)
{
    LOG_MODEL_DEBUG("PJSIP", "%s", data);
}
