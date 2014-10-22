#include "CaptureVideoFrame.h"
#include <QCamera>
#include <QThread>
#include <QTime>
#include <QtDebug>
#include <QCameraInfo>
#include <QVideoFrame>
#include <QVideoSurfaceFormat>
#include <QImage>
#include "FrmVideo.h"
#include "../../Global/Global.h"

CCaptureVideoFrame::CCaptureVideoFrame(QObject *parent) :
    QAbstractVideoSurface(parent)
{
}

CCaptureVideoFrame::~CCaptureVideoFrame()
{
}

//选择需要捕获视频帧的格式  
QList<QVideoFrame::PixelFormat> 
CCaptureVideoFrame::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    LOG_MODEL_DEBUG("CCaptureVideoFrame", "CCaptureVideoFrame::supportedPixelFormats handleType:%d", handleType);
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32//windows 平台、linux 平台默认都支持 RGB32 格式  
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}
/*
bool CCaptureVideoFrame::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();
    LOG_MODEL_DEBUG("CCaptureVideoFrame", "format:%d;size:%s;handleType:%d", imageFormat, size, format.handleType());
    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}
*/
bool CCaptureVideoFrame::present(const QVideoFrame &frame)
{
#ifdef DEBUG_VIDEO_TIME
    static QTime preTime = QTime::currentTime();
    QTime curTime = QTime::currentTime();
    LOG_MODEL_DEBUG("Video", "CCaptureVideoFrame::present:threadid:%d, preTime:%s, currTime:%s, space:%d",
           QThread::currentThreadId(),
           qPrintable(preTime.toString("hh:mm:ss.zzz")),
           qPrintable(curTime.toString("hh:mm:ss.zzz")),
           preTime.msecsTo(curTime));
    preTime = curTime;
#endif
    emit sigCaptureFrame(frame);
    return true;
}

//根据不同的平台，设置捕获方式  
bool CCaptureVideoFrame::setSource(QCamera *pCamera)
{
    bool ret = true;

#ifdef ANDROID
    //android下,目前只能用probe捕获视频  
    ret = m_Probe.setSource(pCamera);
    if(ret)
    {
        ret = connect(&m_Probe, 
                SIGNAL(videoFrameProbed(const QVideoFrame&)),
                SLOT(present(const QVideoFrame&)));
        Q_ASSERT(ret);
    }
    else
        LOG_MODEL_ERROR("CaptureVideo", "m_Probe.setSource fail");
#else
    //windows下,只能用下面方式捕获视频  
    pCamera->setViewfinder(this);
#endif
    return ret;
}
