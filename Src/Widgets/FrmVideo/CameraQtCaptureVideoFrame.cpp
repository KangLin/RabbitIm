#include "CameraQtCaptureVideoFrame.h"
#include "ImageTool.h"

#include <QThread>
#include <QTime>
#include <QVideoFrame>
#include <QImage>
#include <QDebug>

CCameraQtCaptureVideoFrame::CCameraQtCaptureVideoFrame(QObject *parent)
    : QAbstractVideoSurface(parent)
{
    m_Angle = 0;
}

CCameraQtCaptureVideoFrame::~CCameraQtCaptureVideoFrame()
{
}

//选择需要捕获视频帧的格式  
QList<QVideoFrame::PixelFormat> 
CCameraQtCaptureVideoFrame::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB24
                << QVideoFrame::Format_RGB32  //windows 平台、linux 平台默认都支持 RGB32 格式  
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555
                
                << QVideoFrame::Format_BGR32
                << QVideoFrame::Format_BGR24
                << QVideoFrame::Format_BGR565
                << QVideoFrame::Format_BGR555
                << QVideoFrame::Format_BGRA32

                //android支持的格式
                // YUV420SP
                << QVideoFrame::Format_NV12
                << QVideoFrame::Format_NV21
                   
                // YUV420P
                << QVideoFrame::Format_YUV420P
                << QVideoFrame::Format_YV12
                
                << QVideoFrame::Format_UYVY
                << QVideoFrame::Format_YUYV
                << QVideoFrame::Format_YUV444
                << QVideoFrame::Format_AYUV444
                //<< QVideoFrame::Format_Jpeg
                //<< QVideoFrame::Format_CameraRaw
                //<< QVideoFrame::Format_User
                ;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}
/*
bool CCameraQtCaptureVideoFrame::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();
    qDebug() <<  "format:" << format.pixelFormat();
    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}//*/

bool CCameraQtCaptureVideoFrame::present(const QVideoFrame &frame)
{
    emit sigCaptureFrame(frame);

    QImage img = CImageTool::Instance()->ConverFormatToRGB888(frame);
    if(m_Angle)
        img = img.transformed(QTransform().rotate(-1 * m_Angle));
    emit sigCaptureFrame(img);
    return true;
}

int CCameraQtCaptureVideoFrame::SetCameraAngle(int angle)
{
    m_Angle = angle;
    return 0;
}
