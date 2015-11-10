#include "FrameProcess.h"
#include "../../Tool.h"
#include "DataVideoBuffer.h"
#include "../../Global/Global.h"
#include "FrmVideo.h"
#include <QThread>

CFrameProcess::CFrameProcess(QObject *parent) :
    QObject(parent)
{
}

CFrameProcess::~CFrameProcess()
{
}

#ifdef ANDROID
//捕获视频帧。android下是图像格式是NV21,背景摄像头要顺时针旋转90度  
//前景摄像头要逆时针旋转90度  
void CFrameProcess::slotCaptureFrame(
        const std::shared_ptr<CVideoFrame> frame)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess", "CFrameProcess::slotCaptureFrame threadid:0x%X",
           QThread::currentThreadId());
#endif

    if(!frame)
    {
        LOG_MODEL_ERROR("CFrameProcess", "frame is null");
        return;
    }

    do{
        //用opencv捕获摄像头时，进行的处理  
        if(VIDEO_FORMAT_RGB24 == frame->m_VideoInfo.Format)
        {
/*
            //用opencv库做图像镜像  
            cv::Mat src(inFrame.height(), inFrame.width(), CV_8UC3, inFrame.bits());
            cv::Mat dst;
            if(m_pCamera->GetCameraPoistion() == CCamera::BackFace)
            {
                cv::transpose(src, dst);//转置  
                cv::flip(dst, dst, 1); //最后一个参数flip_mode = 0 沿X-轴翻转, flip_mode > 0 (如 1) 沿Y-轴翻转， flip_mode < 0 (如 -1) 沿X-轴和Y-轴翻转  
            }
            else if(m_pCamera->GetCameraPoistion() == CCamera::FrontFace)
            {
                cv::transpose(src, dst);//转置  
                cv::flip(dst, dst, 0);
            }
            else
            {
                LOG_MODEL_DEBUG("CFrameProcess", "camera poistion is error");
                dst = src;
            }
            //dst = CTool::ImageRotate(src, cv::Point(src.cols >> 1, src.rows >> 1), m_pCamera->GetOrientation());//有黑边  
            QImage img((uchar*)(dst.data), dst.cols, dst.rows, QImage::Format_RGB888);  //RGB888就是RGB24即RGB  
*/
#if  RABBITIM_USE_FFMPEG 
            QImage img((const uchar*)frame->GetData(), 
                       frame->m_VideoInfo.nWidth,
                       frame->m_VideoInfo.nHeight,
                       CTool::VideoFormatToQImageFormat(
                           frame->m_VideoInfo.Format));
            QMatrix m;
            img = img.transformed(m.rotate(270));
#endif
            QVideoFrame outFrame(img);
            emit sigCaptureFrame(outFrame);
            break;
        }

        if(VIDEO_FORMAT_NV21 != frame->m_VideoInfo.Format)
        {
            LOG_MODEL_WARNING("Video",
                 "CFrameProcess::slotCaptureFrame:don't Format_NV21");
            CDataVideoBuffer *pBuffer = NULL;
            pBuffer = new CDataVideoBuffer(
                        QByteArray((const char*)frame->GetData(),
                                   frame->GetLength()),
                        frame->m_VideoInfo.nWidth,
                        frame->m_VideoInfo.nHeight);
            QVideoFrame outFrame(pBuffer,
                                 QSize(frame->m_VideoInfo.nWidth,
                                       frame->m_VideoInfo.nHeight),
                                 CTool::VideoFormatToQVideoFrameFormat(
                                     frame->m_VideoInfo.Format));
            emit sigCaptureFrame(outFrame);
            break;
        }

        int nWidth = frame->m_VideoInfo.nWidth;
        int nHeight = frame->m_VideoInfo.nHeight;
        CDataVideoBuffer *pBuffer = NULL;
        QByteArray mirror, rotate;
        mirror.resize(frame->GetLength());
        rotate.resize(frame->GetLength());

        if(CGlobal::Instance()->GetAudioInputDevice() == 1)
        {
            //背景摄像头要顺时针旋转90度,再做Y轴镜像  
            CTool::YUV420spRotate90(reinterpret_cast<uchar *> (rotate.data()),
                                    (const uchar*)frame->GetData(), nWidth, nHeight, 1);
            CTool::YUV420spMirror(reinterpret_cast<uchar *> (mirror.data()),
                                  reinterpret_cast<uchar *>(rotate.data()),
                                  nHeight, nWidth, 0);
            pBuffer = new CDataVideoBuffer(mirror, nHeight, nWidth);
        }
        else
        {
            //前景摄像头要逆时针旋转90度  
            CTool::YUV420spRotate90(
                        reinterpret_cast<uchar *> (
                            rotate.data()), (const uchar*)frame->GetData(),
                        nWidth, nHeight, -1);
            pBuffer = new CDataVideoBuffer(rotate, nHeight, nWidth);
        }
        QVideoFrame outFrame(pBuffer, QSize( nHeight, nWidth),
                             QVideoFrame::Format_NV21);
        emit sigCaptureFrame(outFrame);

    }while(0);

    return;
}
#else
//捕获视频帧。windows下格式是RGB32,做Y轴镜像  
void CFrameProcess::slotCaptureFrame(const std::shared_ptr<CVideoFrame> frame)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess",
                    "CFrameProcess::slotCaptureFrame threadid:0x%X",
           QThread::currentThreadId());
#endif

    if(!frame)
        return;

    do{
        //windows下要镜像  
        /*if(frame->m_VideoInfo.Format != VIDEO_FORMAT_RGB32)
        {
            emit sigCaptureFrame(frame);
            break;
        }*/

#ifdef RABBITIM_USE_OPENCV
        //*用opencv库做图像镜像  
        int nType = CV_8UC4;
        if(VIDEO_FORMAT_RGB24 == frame->m_VideoInfo.Format)
            nType = CV_8UC3;
        QByteArray outData;
        outData.resize(frame->GetLength());//dst.total指图片像素个数，总字节数(dst.data)=dst.total*dst.channels()  
        cv::Mat src(frame->m_VideoInfo.nHeight,
                    frame->m_VideoInfo.nWidth,
                    nType, frame->GetData());
        cv::Mat dst(frame->m_VideoInfo.nHeight,
                    frame->m_VideoInfo.nWidth,
                    nType, outData.data());
        cv::flip(src, dst, 1);  //最后一个参数flip_mode = 0 沿X-轴翻转, flip_mode > 0 (如 1) 沿Y-轴翻转， flip_mode < 0 (如 -1) 沿X-轴和Y-轴翻转  
        //dst = CTool::ImageRotate(src, cv::Point(inFrame.width() >> 1, inFrame.height() >> 1), m_pCamera->GetOrientation());  

        //由QVideoFrame进行释放  
        CDataVideoBuffer* pBuffer = new CDataVideoBuffer(outData,
                                                         dst.cols,
                                                         dst.rows);
        QVideoFrame outFrame(pBuffer,
                             QSize(dst.cols,
                                   dst.rows),
                             CTool::VideoFormatToQVideoFrameFormat(
                                       frame->m_VideoInfo.Format));//*/
#else
        //用QImage做图像镜像  
        QImage img((const uchar*)frame->GetData(),
                   frame->m_VideoInfo.nWidth,
                   frame->m_VideoInfo.nHeight,
                   CTool::VideoFormatToQImageFormat(
                       frame->m_VideoInfo.Format));
        img = img.mirrored(false, false);
        //由QVideoFrame进行释放  
        CDataVideoBuffer* pBuffer = new CDataVideoBuffer(
                    QByteArray((const char*)img.bits(),
                               img.byteCount()),
                    frame->m_VideoInfo.nWidth,
                    frame->m_VideoInfo.nHeight
                    );
        QVideoFrame outFrame(pBuffer,
                             QSize(frame->m_VideoInfo.nWidth,
                                 frame->m_VideoInfo.nHeight),
                       CTool::VideoFormatToQVideoFrameFormat(
                                 frame->m_VideoInfo.Format));
#endif
        emit sigCaptureFrame(outFrame);
    }while(0);

    return;
}
#endif

void CFrameProcess::slotFrameConvertedToYUYV(
        const QVideoFrame &frame, int nWidth, int nHeight)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess",
                    "CFrameProcess::slotFrameConvertedToYUYV threadid:0x%X",
           QThread::currentThreadId());
#endif
    QVideoFrame inFrame(frame);
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
        return;

    if(-1 == nWidth)
        nWidth = inFrame.width();
    if(-1 == nHeight)
        nHeight = inFrame.height();

    do{
#if  RABBITIM_USE_FFMPEG && RABBITIM_USE_QXMPP
        //转换图片格式
        AVPicture pic;
        int nRet = 0;
        nRet = CTool::ConvertFormat(inFrame, pic, nWidth, nHeight,
                                    AV_PIX_FMT_YUYV422);
        if(nRet)
        {
            LOG_MODEL_ERROR("Video", "CTool::ConvertFormat fail");
            break;
        }

        int size = avpicture_get_size(AV_PIX_FMT_YUYV422, nWidth, nHeight);
        QSize sizeFrame(nWidth, nHeight);
        QXmppVideoFrame f(size, sizeFrame, size / nHeight,
                          QXmppVideoFrame::Format_YUYV);
        nRet = avpicture_layout(&pic, AV_PIX_FMT_YUYV422,
                                nWidth, nHeight, f.bits(), size);
        if(nRet > 0)
        {
            emit sigFrameConvertedToYUYVFrame(f);
        }

        avpicture_free(&pic);
#else
#error "Must use ffmpeg library and qxmpp library"
#endif//#ifdef  RABBITIM_USE_FFMPEG && RABBITIM_USE_QXMPP
    }while(0);

    inFrame.unmap();
}

void CFrameProcess::slotFrameConvertedToRGB32(
        const QVideoFrame &inFrame, QRect rect)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess",
                    "CFrameProcess::slotFrameConvertedToRGB32 threadid:0x%X",
           QThread::currentThreadId());
#endif
    QVideoFrame outFrame;
    QVideoFrame f(inFrame);

    if(inFrame.pixelFormat() == QVideoFrame::Format_BGR32
            && (rect.isEmpty()
                || (rect.width() == f.width()
                    && rect.height() == f.height())))
    {
        emit sigFrameConvertedToRGB32Frame(inFrame);
        return;
    }

    //QVideoFrame使用bits前一定要先map，bits才会生效  
    if(!f.map(QAbstractVideoBuffer::ReadOnly))
        return;

    if(rect.isEmpty())
    {
        rect.setWidth(f.width());
        rect.setHeight(f.height());
    }

    do
    {
#ifdef RABBITIM_USE_FFMPEG
        //图片格式转换  
        AVPicture pic;
        LOG_MODEL_DEBUG("CFrmeProcess",
                        "width:%d;height:%d;f.width:%d;height:%d",
                        rect.width(), rect.height(), f.width(), f.height());
        int nRet = CTool::ConvertFormat(f, pic, rect.width(), rect.height(),
                                        AV_PIX_FMT_RGB32);
        if(nRet)
            break;

        FillFrame(pic, rect, outFrame);

        avpicture_free(&pic);
#else
#error "Must use ffmpeg or opencv library"
#endif
    }while(0);

    f.unmap();

    emit sigFrameConvertedToRGB32Frame(outFrame);
}

#ifdef RABBITIM_USE_QXMPP
void CFrameProcess::slotFrameConvertedToRGB32(
        const QXmppVideoFrame &frame, QRect rect)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess",
                    "CFrameProcess::slotFrameConvertedToRGB32 threadid:0x%X",
           QThread::currentThreadId());
#endif
    if(rect.isEmpty())
    {
        rect.setWidth(frame.width());
        rect.setHeight(frame.height());
    }

    QVideoFrame outFrame;
#ifdef RABBITIM_USE_FFMPEG
    //图片格式转换  
    AVPicture pic;
    int nRet = CTool::ConvertFormat(frame, pic, rect.width(),
                                    rect.height(), AV_PIX_FMT_RGB32);
    if(nRet)
        return;
    FillFrame(pic, rect, outFrame);
    avpicture_free(&pic);
#else
#error "Must use ffmpeg or opencv library"
#endif
    emit sigFrameConvertedToRGB32Frame(outFrame);
}
#endif

#ifdef RABBITIM_USE_FFMPEG
int CFrameProcess::FillFrame(const AVPicture &pic,
                             const QRect &rect, QVideoFrame &frame)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess", "CFrameProcess::FillFrame threadid:0x%X",
           QThread::currentThreadId());
#endif
    int nRet = 0;
    int size = avpicture_get_size(AV_PIX_FMT_RGB32,
                                  rect.width(), rect.height());

    QByteArray outFrame;
    outFrame.resize(size);
    CDataVideoBuffer *pBuf = new CDataVideoBuffer(outFrame,
                                                  rect.width(), rect.height());
    QVideoFrame f(pBuf, QSize(rect.width(), rect.height()),
                  QVideoFrame::Format_RGB32);
    frame = f;
    if(frame.map(QAbstractVideoBuffer::WriteOnly))
    {
        avpicture_layout(&pic, AV_PIX_FMT_RGB32,
                         rect.width(), rect.height(),
                         frame.bits(), frame.mappedBytes());
        frame.unmap();
    }
    else
        return -1;
    return nRet;
}

int CFrameProcess::FillFrame(const std::shared_ptr<CVideoFrame> inFrame,
                             QVideoFrame &outFrame)
{
    QByteArray ba((const char*)inFrame->GetData(), inFrame->GetLength());
    CDataVideoBuffer *pBuf = new CDataVideoBuffer(ba,
                                 inFrame->m_VideoInfo.nWidth,
                                 inFrame->m_VideoInfo.nHeight);
    QVideoFrame f(pBuf, QSize(inFrame->m_VideoInfo.nWidth,
                              inFrame->m_VideoInfo.nHeight),
                  QVideoFrame::Format_RGB32);
    outFrame = f;
    return 0;
}

#endif
