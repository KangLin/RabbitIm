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
    CCameraQt(int nIndex, QObject *parent = 0);
    virtual ~CCameraQt();

    virtual int Open(CHanderFrame* pHander, VideoInfo* pVideoInfo = NULL);
    virtual int Close();
    virtual int Start();
    virtual int Stop();

private:
    std::auto_ptr<QCamera> m_Camera;
    CCameraQtCaptureVideoFrame m_CaptureFrame;
    friend class CCameraQtCaptureVideoFrame;
    bool Present(const QVideoFrame &frame);
    VideoFormat QVideoFrameFormatToVideoFormat(
            const QVideoFrame::PixelFormat format);
};

#endif // CCAMERAQT_H
