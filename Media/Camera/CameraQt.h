#ifndef CCAMERAQT_H
#define CCAMERAQT_H

#include "Camera.h"
#include "CameraQtCaptureVideoFrame.h"
#include <QCamera>
#include <QCameraImageCapture>

/**
 * @brief The CCameraQt class
 * @ingroup RABBITIM_IMPLEMENT_CAMERA_QT
 * @see CCamera
 */
class CCameraQt : public CCamera
{
    Q_OBJECT
public:
    CCameraQt(int nIndex);
    virtual ~CCameraQt();

    virtual int Start();
    virtual int Stop();
    virtual int Capture(const std::string &szFileName);

private:
    virtual int OnOpen(VideoInfo* pVideoInfo = NULL);
    virtual int OnClose();
private slots:
    void imageSaved(int id, const QString & fileName);
    
private:
    QCamera* m_pCamera;
    QCameraImageCapture* m_pCameraImageCapture;
    int m_CameraImageCaptureID;
    CCameraQtCaptureVideoFrame m_CaptureFrame;
    friend class CCameraQtCaptureVideoFrame;
    bool Present(const QVideoFrame &frame);
    VideoFormat QVideoFrameFormatToVideoFormat(
            const QVideoFrame::PixelFormat format);
};

#endif // CCAMERAQT_H
