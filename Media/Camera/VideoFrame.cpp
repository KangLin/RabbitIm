#include "VideoFrame.h"

CVideoFrame::CVideoFrame(const void *pData, const long &nLen, 
                         const VideoInfo &videoInfo, const long &timestamp)
{
    m_VideoInfo = videoInfo;
    m_Timestamp = timestamp;
    m_nLen = nLen;
    if(pData && nLen > 0)
        m_pData = new char[m_nLen];
    if(m_pData)
        memcpy(m_pData, pData, m_nLen);
}

CVideoFrame::CVideoFrame()
{
    
}

CVideoFrame::~CVideoFrame()
{
    //LOG_MODEL_DEBUG("CVideoFrame", "CVideoFrame::~CVideoFrame:time:%d", m_Timestamp);
    if(m_pData)
        delete []m_pData;
}

const void* CVideoFrame::GetData()
{
    return m_pData;
}

long CVideoFrame::GetLength()
{
    return m_nLen;
}
