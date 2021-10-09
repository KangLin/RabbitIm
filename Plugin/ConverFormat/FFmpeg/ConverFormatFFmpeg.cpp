#include "ConverFormatFFmpeg.h"
#include "RabbitCommonLog.h"

//设置日志的回调函数
void Log(void*, int, const char* fmt, va_list vl)
{
    LOG_MODEL_DEBUG("ffmpeg", fmt, vl);
}

CConverFormatFFmpeg::CConverFormatFFmpeg(QObject *parent) : CPluginConverFormat(parent)
{
    SetFFmpegLog();
}

int CConverFormatFFmpeg::SetFFmpegLog()
{
    //在程序初始化时设置ffmpeg日志的回调函数
    av_log_set_callback(Log);
    return 0;
}

AVPixelFormat CConverFormatFFmpeg::QVideoFrameFormatToFFMpegPixFormat(
        const QVideoFrame::PixelFormat format)
{
    switch (format) {
    case QVideoFrame::Format_RGB32:
        return AV_PIX_FMT_RGB32;
    case QVideoFrame::Format_RGB24:
        return AV_PIX_FMT_RGB24;
    case QVideoFrame::Format_YUYV:
        return AV_PIX_FMT_YUYV422;
    case QVideoFrame::Format_UYVY:
        return AV_PIX_FMT_UYVY422;
    case QVideoFrame::Format_NV21:
        return AV_PIX_FMT_NV21;
    case QVideoFrame::Format_NV12:
        return AV_PIX_FMT_NV12;
    case QVideoFrame::Format_YUV420P:
        return AV_PIX_FMT_YUV420P;
    case QVideoFrame::Format_YV12:
    default:
        LOG_MODEL_ERROR("CConverFormatFFmpeg",  "Don't conver format: %d", format);
        return AV_PIX_FMT_NONE;
    }
}

AVPixelFormat CConverFormatFFmpeg::QImageFormatToFFMpegPixFormat(const QImage::Format format)
{
    if(QImage::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    return AV_PIX_FMT_NONE;
}

//如果转换成功，则调用者使用完 pOutFrame 后，需要调用 avpicture_free(pOutFrame) 释放内存
//成功返回0，不成功返回非0
int CConverFormatFFmpeg::ConvertFormat(/*[in]*/ const AVPicture &inFrame,
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

QImage CConverFormatFFmpeg::onConverFormatToRGB888(const QVideoFrame &frame)
{
    int nRet = 0;
    AVPicture inPic, outPic;
    QImage img;
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return img;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        return img;

    do{
        img = QImage(videoFrame.width(),
                     videoFrame.height(),
                     QImage::Format_RGB888);
        nRet = avpicture_fill(&outPic, img.bits(),
                              AV_PIX_FMT_RGB24,
                              img.width(),
                              img.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CConverFormatFFmpeg", "avpicture_get_size fail:%d", nRet);
            break;
        }
        nRet = avpicture_fill(&inPic, videoFrame.bits(),
                              QVideoFrameFormatToFFMpegPixFormat(videoFrame.pixelFormat()),
                              videoFrame.width(),
                              videoFrame.height());
        if(nRet < 0)
        {
            LOG_MODEL_ERROR("CConverFormatFFmpeg", "avpicture_fill is fail");
            break;
        }

        nRet = ConvertFormat(inPic, videoFrame.width(), videoFrame.height(),
                             QVideoFrameFormatToFFMpegPixFormat(videoFrame.pixelFormat()),
                             outPic, img.width(), img.height(),
                             AV_PIX_FMT_RGB24);
    }while (0);
    videoFrame.unmap();
    return img;
}
