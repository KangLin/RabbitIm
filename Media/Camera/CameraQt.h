#ifndef CCAMERAQT_H
#define CCAMERAQT_H

#include "Camera.h"
#include "CameraQtCaptureVideoFrame.h"
#include <QCamera>

/**
 * @brief The CCameraQt class
 * @ingroup RABBITIM_IMPLEMENT_CAMERA_QT
 * @see CCamera
 */
class CCameraQt : public CCamera
{
public:
    CCameraQt(int nIndex);
    virtual ~CCameraQt();

    virtual int Start();
    virtual int Stop();

private:
    virtual int OnOpen(VideoInfo* pVideoInfo = NULL);
    virtual int OnClose();

private:
    std::auto_ptr<QCamera> m_Camera;
    CCameraQtCaptureVideoFrame m_CaptureFrame;
    friend class CCameraQtCaptureVideoFrame;
    bool Present(const QVideoFrame &frame);
    VideoFormat QVideoFrameFormatToVideoFormat(
            const QVideoFrame::PixelFormat format);
};

#endif // CCAMERAQT_H
