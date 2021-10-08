#include "ConvertFormat.h"
#include "RabbitCommonLog.h"

CConvertFormat::CConvertFormat() : CTool()
{}

AVPixelFormat CConvertFormat::QXmppVideoFrameFormatToFFMpegPixFormat(
        const QXmppVideoFrame::PixelFormat format)
{
    if(QXmppVideoFrame::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    else if(QXmppVideoFrame::Format_RGB24 == format)
        return AV_PIX_FMT_RGB24;
    else if(QXmppVideoFrame::Format_YUYV == format)
        return AV_PIX_FMT_YUYV422;
    else if(QXmppVideoFrame::Format_UYVY == format)
        return AV_PIX_FMT_UYVY422;
    else if(QXmppVideoFrame::Format_YUV420P == format)
        return AV_PIX_FMT_YUV420P;
    else
        return AV_PIX_FMT_NONE;
}

QXmppVideoFrame::PixelFormat CConvertFormat::QVideoFrameFormatToQXmppVideoFrameFormat(
        const QVideoFrame::PixelFormat format)
{
    if(QVideoFrame::Format_RGB32 == format)
        return QXmppVideoFrame::Format_RGB32;
    else if(QVideoFrame::Format_RGB24 == format)
        return QXmppVideoFrame::Format_RGB24;
    else if(QVideoFrame::Format_YUYV == format)
        return QXmppVideoFrame::Format_YUYV;
    else if(QVideoFrame::Format_UYVY == format)
        return QXmppVideoFrame::Format_UYVY;
    else if(QVideoFrame::Format_YUV420P == format)
        return QXmppVideoFrame::Format_YUV420P;
    else
        return QXmppVideoFrame::Format_Invalid;
}

QVideoFrame::PixelFormat CConvertFormat::QXmppVideoFrameFormatToQVideoFrameFormat(
        const QXmppVideoFrame::PixelFormat format)
{
    if(QXmppVideoFrame::Format_RGB32 == format)
        return QVideoFrame::Format_RGB32;
    else if(QXmppVideoFrame::Format_RGB24 == format)
        return QVideoFrame::Format_RGB24;
    else if(QXmppVideoFrame::Format_YUYV == format)
        return QVideoFrame::Format_YUYV;
    else if(QXmppVideoFrame::Format_UYVY == format)
        return QVideoFrame::Format_UYVY;
    else if(QXmppVideoFrame::Format_YUV420P == format)
        return QVideoFrame::Format_YUV420P;
    else
        return QVideoFrame::Format_Invalid;
}

int CConvertFormat::ConvertFormat(const QXmppVideoFrame &inFrame,
                         QVideoFrame &outFrame,
                         int nOutWidth,
                         int nOutHeight,
                         QVideoFrame::PixelFormat outPixelFormat)
{
    int nRet = 0;
    AVPicture inPic, outPic;
    nRet = avpicture_fill(&inPic, (uint8_t*)inFrame.bits(),
                          QXmppVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()),
                          inFrame.width(),
                          inFrame.height());
    if(nRet < 0)
    {
        LOG_MODEL_ERROR("CTool", "avpicture_fill is fail");
        return nRet;
    }

    int nByte = avpicture_fill(&outPic, NULL,
                          QVideoFrameFormatToFFMpegPixFormat(outPixelFormat),
                          nOutWidth,
                          nOutHeight);
    if(nByte < 0)
    {
        LOG_MODEL_ERROR("CTool", "avpicture_get_size fail:%d", nByte);
        return -2;
    }
    QVideoFrame out(nByte, QSize(nOutWidth, nOutHeight), outPic.linesize[0], outPixelFormat);
    if(!out.map(QAbstractVideoBuffer::WriteOnly))
    {
        LOG_MODEL_ERROR("CTool", "outFrame map is fail");
        return -3;
    }
    do{
        nRet = avpicture_fill(&outPic, out.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(out.pixelFormat()),
                              out.width(),
                              out.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_file is fail:%d", nRet);
            nRet = -4;
            break;
        }
        nRet = CTool::ConvertFormat(inPic, inFrame.width(), inFrame.height(),
                             QXmppVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()),
                             outPic, out.width(), out.height(),
                             QVideoFrameFormatToFFMpegPixFormat(outPixelFormat));
        if(!nRet)
            outFrame = out;
    }while(0);
    out.unmap();
    return nRet;
}

int CConvertFormat::ConvertFormat(const QVideoFrame &inFrame,
                         QXmppVideoFrame &outFrame,
                         int nOutWidth,
                         int nOutHeight,
                         QVideoFrame::PixelFormat outPixelFormat)
{
    int nRet = 0;
    QVideoFrame in(inFrame);
    if(!in.map(QAbstractVideoBuffer::ReadOnly))
    {
        LOG_MODEL_ERROR("CTool", "map is fail");
        return -1;
    }

    do{
        AVPicture inPic, outPic;
        nRet = avpicture_fill(&inPic, in.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(in.pixelFormat()),
                              in.width(),
                              in.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_fill is fail");
            return nRet;
        }
        /*int nByte = avpicture_get_size(QVideoFrameFormatToFFMpegPixFormat(outPixelFormat), nOutWidth, nOutHeight);
        if(nByte < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_get_size fail:%d", nByte);
            nRet = -2;
            break;
        }*/
        int nByte = avpicture_fill(&outPic, NULL,
                              QVideoFrameFormatToFFMpegPixFormat(outPixelFormat),
                              nOutWidth,
                              nOutHeight);
        if(nByte < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_get_size fail:%d", nByte);
            nRet = -2;
            break;
        }
        QXmppVideoFrame out(nByte,
                            QSize(nOutWidth, nOutHeight),
                            outPic.linesize[0],
                QVideoFrameFormatToQXmppVideoFrameFormat(outPixelFormat));
        
        nRet = avpicture_fill(&outPic, out.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(outPixelFormat),
                              out.width(),
                              out.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_fill is fail");
            nRet = -4;
            break;
        }
        nRet = CTool::ConvertFormat(inPic, inFrame.width(), inFrame.height(),
                             QVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()),
                             outPic, out.width(), out.height(),
                             QVideoFrameFormatToFFMpegPixFormat(outPixelFormat));
        if(nRet)
        {
            LOG_MODEL_ERROR("CTool", "CTool::ConvertFormat is fail");
            break;
        }
        outFrame = out;
        nRet = 0;
    }while(0);

    in.unmap();
    return nRet;
}
