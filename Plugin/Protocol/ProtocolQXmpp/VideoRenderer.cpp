#include "VideoRenderer.h"
#include <QByteArray>
#include "talk/media/base/videocommon.h"
#include "RabbitCommonLog.h"
#include "Widgets/FrmVideo/DataVideoBuffer.h"
#include "CallObjectQXmppWebrtc.h"
#include <QImage>

CVideoRenderer::CVideoRenderer(webrtc::VideoTrackInterface *track_to_render,
                               CCallObjectQXmppWebrtc* pCallObject,
                               bool isLocale)
    : m_Track(track_to_render),
      m_pCallObject(pCallObject),
      m_isLocale(isLocale)
{
    m_BitmapSize = 0;
    m_Height = 0;
    m_Width = 0;
    m_Track->AddRenderer(this);
}

CVideoRenderer::~CVideoRenderer()
{
    m_Track->RemoveRenderer(this);
}

void CVideoRenderer::SetSize(int width, int height)
{
    //ARGB格式  
    if(width == m_Height && height == m_Height)
        return;
    m_Height = height;
    m_Width = width;
    m_BitmapSize = m_Height * m_Width * 4;
}

void CVideoRenderer::RenderFrame(const cricket::VideoFrame *video_frame)
{
    if (!video_frame)
        return;

    const cricket::VideoFrame* frame =
            video_frame->GetCopyWithRotationApplied();

    SetSize(static_cast<int>(frame->GetWidth()),
            static_cast<int>(frame->GetHeight()));

    QImage outFrame(frame->GetWidth(), frame->GetHeight(), QImage::Format_ARGB32);
    //ARGB格式  
    frame->ConvertToRgbBuffer(cricket::FOURCC_ARGB,
                              (uint8_t*)outFrame.bits(),
                              m_BitmapSize,
                              m_Width << 2);

    if(m_isLocale && m_pCallObject)
        emit m_pCallObject->sigRenderLocale(outFrame);
    else
        emit m_pCallObject->sigRenderRemote(outFrame);
}
