#include "Tool.h"

CTool::CTool(QObject *parent) :
    QObject(parent)
{
}

CTool::~CTool()
{
}


//设置日志的回调函数
void Log(void*, int, const char* fmt, va_list vl)
{
    static char buf[1024];
    vsprintf(buf, fmt, vl);
    qDebug(buf);
}

int CTool::SetFFmpegLog()
{
    //在程序初始化时设置ffmpeg日志的回调函数
    av_log_set_callback(Log);
    return 0;
}

AVPixelFormat CTool::QVideoFrameFormatToFFMpegPixFormat(const QVideoFrame::PixelFormat format)
{
    if(QVideoFrame::Format_RGB32 == format)
        return AV_PIX_FMT_RGB32;
    else if(QVideoFrame::Format_BGR24 == format)
        return AV_PIX_FMT_BGR24;
    else if(QVideoFrame::Format_RGB24 == format)
        return AV_PIX_FMT_RGB24;
    else if(QVideoFrame::Format_YUYV == format)
        return AV_PIX_FMT_YUYV422;
    else if(QVideoFrame::Format_NV21 == format)
        return AV_PIX_FMT_NV21;
    else
        return AV_PIX_FMT_NONE;
}

//如果转换成功，则调用者使用完 pOutFrame 后，需要调用 avpicture_free(pOutFrame) 释放内存
//成功返回0，不成功返回非0
int CTool::ConvertFormat(/*[in]*/const QVideoFrame &inFrame,
                              /*[out]*/AVPicture *pOutFrame,
                              AVPixelFormat pixelFormat)
{
    int nRet = 0;
    struct SwsContext* pSwsCtx = NULL;

    AVPicture pic;

    nRet = avpicture_fill(&pic, inFrame.bits(),
                   QVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()),
                   inFrame.width(),
                   inFrame.height());
    if(nRet < 0)
    {
        qDebug(("avpicture_fill fail:%x\n"), nRet);
        return nRet;
    }

    //分配输出空间
    nRet = avpicture_alloc(pOutFrame, pixelFormat, inFrame.width(), inFrame.height());
    if(nRet)
    {
        qDebug(("avpicture_alloc fail:%x\n"), nRet);
        return nRet;
    }

    if(QVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()) == pixelFormat)
    {
        av_picture_copy(pOutFrame, &pic, pixelFormat, inFrame.width(), inFrame.height());
        return 0;
    }

    //设置图像转换上下文
    pSwsCtx = sws_getCachedContext (NULL,
                                    inFrame.width(),                //源宽度
                                    inFrame.height(),               //源高度
                                    QVideoFrameFormatToFFMpegPixFormat(inFrame.pixelFormat()),  //源格式
                                    inFrame.width(),                //目标宽度
                                    inFrame.height(),               //目标高度
                                    pixelFormat,                    //目的格式
                                    SWS_FAST_BILINEAR,              //转换算法
                                    NULL, NULL, NULL);
    if(NULL == pSwsCtx)
    {
        qDebug("sws_getContext false\n");
        avpicture_free(pOutFrame);
        return -3;
    }

    //进行图片转换
    nRet = sws_scale(pSwsCtx,
                     pic.data, pic.linesize,
                     0, inFrame.height(),
                     pOutFrame->data, pOutFrame->linesize);
    if(nRet < 0)
    {
        qDebug("sws_scale fail:%x\n", nRet);
        avpicture_free(pOutFrame);
    }
    else
        nRet = 0;

    sws_freeContext(pSwsCtx);

    return nRet;
}
