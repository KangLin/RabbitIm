#ifndef CVIDEORENDERER_H
#define CVIDEORENDERER_H

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/media/base/videoframe.h"

class CCallObjectQXmppWebrtc;
class CVideoRenderer : public webrtc::VideoRendererInterface
{    
public:
    CVideoRenderer(webrtc::VideoTrackInterface* track_to_render, CCallObjectQXmppWebrtc* pCallObject, bool isLocale = true);
    virtual ~CVideoRenderer();
    
    // VideoRendererInterface implementation
    virtual void SetSize(int width, int height);
    virtual void RenderFrame(const cricket::VideoFrame* video_frame);
    
private:
    int m_Width;
    int m_Height;
    int m_BitmapSize;
    rtc::scoped_refptr<webrtc::VideoTrackInterface> m_Track;
    CCallObjectQXmppWebrtc* m_pCallObject;
    bool m_isLocale;
};

#endif // CVIDEORENDERER_H
