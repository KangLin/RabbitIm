#include "CaptureVideoFrame.h"

CCaptureVideoFrame::CCaptureVideoFrame(QObject *parent) :
    QAbstractVideoSurface(parent)
{
}


QList<QVideoFrame::PixelFormat> CCaptureVideoFrame::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    QList<QVideoFrame::PixelFormat> lst;

    //lst.push_back(QVideoFrame::Format_YUYV);//好象Qt现在不支持此格式
    lst.push_back(QVideoFrame::Format_RGB32);

    return lst;
}

bool CCaptureVideoFrame::present(const QVideoFrame &frame)
{
    emit CaptureFrame(frame);

    return true;
}
