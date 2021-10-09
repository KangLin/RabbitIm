#include "ConvertFormat.h"
#include "RabbitCommonLog.h"

CConvertFormat::CConvertFormat()
{}

//设置日志的回调函数  
void Log(void*, int, const char* fmt, va_list vl)
{
    LOG_MODEL_DEBUG("ffmpeg", fmt, vl);
}

int CConvertFormat::SetFFmpegLog()
{
    //在程序初始化时设置ffmpeg日志的回调函数  
    av_log_set_callback(Log);
    return 0;
}

AVPixelFormat CConvertFormat::QVideoFrameFormatToFFMpegPixFormat(
        const QVideoFrame::PixelFormat format)
{
    if(QVideoFrame::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    else if(QVideoFrame::Format_BGR24 == format)
        return AV_PIX_FMT_BGR24;
    else if(QVideoFrame::Format_RGB24 == format)
        return AV_PIX_FMT_RGB24;
    else if(QVideoFrame::Format_YUYV == format)
        return AV_PIX_FMT_YUYV422;
    else if(QVideoFrame::Format_UYVY == format)
        return AV_PIX_FMT_UYVY422;
    else if(QVideoFrame::Format_NV21 == format)
        return AV_PIX_FMT_NV21;
    else if(QVideoFrame::Format_NV12 == format)
        return AV_PIX_FMT_NV12;
    else if(QVideoFrame::Format_YUV444 == format)
        return AV_PIX_FMT_YUYV422;

    return AV_PIX_FMT_NONE;
}

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

AVPixelFormat CConvertFormat::QImageFormatToFFMpegPixFormat(const QImage::Format format)
{
    if(QImage::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    return AV_PIX_FMT_NONE;
}

int CConvertFormat::ConvertFormat(const QVideoFrame &inFrame,
                         QVideoFrame &outFrame,
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
        QVideoFrame out(nByte, QSize(nOutWidth, nOutHeight), outPic.linesize[0], outPixelFormat);
        if(!out.map(QAbstractVideoBuffer::WriteOnly))
        {
            LOG_MODEL_ERROR("CTool", "outFrame map is fail");
            nRet = -3;
            break;
        }
        nRet = avpicture_fill(&outPic, out.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(out.pixelFormat()),
                              out.width(),
                              out.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CTool", "avpicture_fill is fail");
            nRet = -4;
            break;
        }
        nRet = ConvertFormat(inPic, inFrame.width(), inFrame.height(),
                             QVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()),
                             outPic, out.width(), out.height(),
                             QVideoFrameFormatToFFMpegPixFormat(outPixelFormat));
        out.unmap();
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

//如果转换成功，则调用者使用完 pOutFrame 后，需要调用 avpicture_free(pOutFrame) 释放内存  
//成功返回0，不成功返回非0  
int CConvertFormat::ConvertFormat(/*[in]*/ const AVPicture &inFrame,
                         /*[in]*/ int nInWidth,
                         /*[in]*/ int nInHeight,
                         /*[in]*/ AVPixelFormat inPixelFormat,
                         /*[out]*/AVPicture &outFrame,
                         /*[in]*/ int nOutWidth,
                         /*[in]*/ int nOutHeight,
                         /*[in]*/ AVPixelFormat outPixelFormat)
{
    int nRet = 0;
    struct SwsContext* pSwsCtx = NULL;
    
    //分配输出空间  
    /*nRet = avpicture_alloc(&outFrame, outPixelFormat, nOutWidth, nOutHeight);
    if(nRet)
    {
        LOG_MODEL_ERROR("Tool", "avpicture_alloc fail:%x", nRet);
        return nRet;
    }*/
    
    if(inPixelFormat == outPixelFormat
            && nInWidth == nOutWidth
            && nInHeight == nOutHeight)
    {
        av_picture_copy(&outFrame, &inFrame, inPixelFormat,
                        nInWidth, nInHeight);
        return 0;
    }
    
    //设置图像转换上下文  
    pSwsCtx = sws_getCachedContext (NULL,
                                    nInWidth,                //源宽度  
                                    nInHeight,               //源高度  
                                    inPixelFormat,           //源格式  
                                    nOutWidth,               //目标宽度  
                                    nOutHeight,              //目标高度  
                                    outPixelFormat,          //目的格式  
                                    SWS_FAST_BILINEAR,       //转换算法  
                                    NULL, NULL, NULL);
    if(NULL == pSwsCtx)
    {
        LOG_MODEL_ERROR("Tool", "sws_getContext false");
        return -3;
    }
    
    //进行图片转换  
    nRet = sws_scale(pSwsCtx,
                     inFrame.data, inFrame.linesize,
                     0, nInHeight,
                     outFrame.data, outFrame.linesize);
    if(nRet < 0)
    {
        LOG_MODEL_ERROR("Tool", "sws_scale fail:%x", nRet);
    }
    else
    {
        nRet = 0;
    }
    
    sws_freeContext(pSwsCtx);
    return nRet;
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
        nRet = ConvertFormat(inPic, inFrame.width(), inFrame.height(),
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
        nRet = ConvertFormat(inPic, inFrame.width(), inFrame.height(),
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
