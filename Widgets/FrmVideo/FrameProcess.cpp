#include "FrameProcess.h"
#include "Tool.h"
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
        const QVideoFrame &frame)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess", "CFrameProcess::slotCaptureFrame threadid:0x%X",
           QThread::currentThreadId());
#endif

    if(!frame.isValid())
    {
        LOG_MODEL_ERROR("CFrameProcess", "frame is null");
        return;
    }
    QVideoFrame inFrame(frame);
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        LOG_MODEL_ERROR("CFrameProcess", "QVideoFrame inFrame map is fail");
        return;
    }
    
    do{
        //用opencv捕获摄像头时，进行的处理  
        if(QVideoFrame::Format_RGB24 == frame.pixelFormat())
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
            QImage img(frame.bits(), 
                       frame.width(),
                       frame.height(),
                       QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
            QMatrix m;
            img = img.transformed(m.rotate(270));

            QVideoFrame outFrame(img);
            emit sigCaptureFrame(outFrame);
            break;
        }

        if(QVideoFrame::Format_NV21 != frame.pixelFormat())
        {
            LOG_MODEL_WARNING("Video",
                 "CFrameProcess::slotCaptureFrame:don't Format_NV21");
            emit sigCaptureFrame(frame);
            break;
        }

        int nWidth = frame.width();
        int nHeight = frame.height();
        QVideoFrame outFrame(inFrame.mappedBytes(), inFrame.size(),
                             inFrame.bytesPerLine(), inFrame.pixelFormat());
        if(!outFrame.map(QAbstractVideoBuffer::WriteOnly))
        {
             LOG_MODEL_ERROR("CFrameProcess", "QVideoFrame outFrame map is fail");
            break;
        }
        if(CGlobal::Instance()->GetVideoCaptureDevice() == 1)
        {
            QByteArray rotate;
            rotate.resize(frame.mappedBytes());
            //背景摄像头要顺时针旋转90度,再做Y轴镜像  
            CTool::YUV420spRotate90(reinterpret_cast<uchar *> (rotate.data()),
                                    (const uchar*)frame.bits(), nWidth, nHeight, 1);
            CTool::YUV420spMirror(reinterpret_cast<uchar *> (outFrame.bits()),
                                  reinterpret_cast<uchar *>(rotate.data()),
                                  nHeight, nWidth, 0);
        }
        else
        {
            //前景摄像头要逆时针旋转90度  
            CTool::YUV420spRotate90(
                        reinterpret_cast<uchar *> (
                            outFrame.bits()), (const uchar*)frame.bits(),
                        nWidth, nHeight, -1);
        }
        outFrame.unmap();
        emit sigCaptureFrame(outFrame);

    }while(0);

    inFrame.unmap();
    return;
}
#else
//捕获视频帧。windows下格式是RGB32,做Y轴镜像  
void CFrameProcess::slotCaptureFrame(const QVideoFrame &frame)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess",
                    "CFrameProcess::slotCaptureFrame threadid:0x%X",
           QThread::currentThreadId());
#endif

    if(!frame.isValid())
        return;
    
    if(!(frame.pixelFormat() == QVideoFrame::Format_RGB24
            || frame.pixelFormat() == QVideoFrame::Format_RGB32))
    {
        LOG_MODEL_WARNING("CFrameProcess", "frame format isn't RGB24 or RGB32");
        emit sigCaptureFrame(frame);
        return;
    }
    
    QVideoFrame inFrame(frame);

    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        LOG_MODEL_ERROR("CFrameProcess", "outFrame map is fail");
        return;
    }
    
    do{
        //windows下要镜像   
#ifdef RABBITIM_USE_OPENCV
        QVideoFrame outFrame(inFrame.mappedBytes(), inFrame.size(),
                             inFrame.bytesPerLine(), inFrame.pixelFormat());
        if(outFrame.map(QAbstractVideoBuffer::WriteOnly))
        {
            break;
        }
        //*用opencv库做图像镜像  
        int nType = CV_8UC4;
        if(inFrame.pixelFormat() == QVideoFrame::Format_RGB24)
            nType = CV_8UC3;
        QByteArray outData;
        outData.resize(outFrame.mappedBytes());//dst.total指图片像素个数，总字节数(dst.data)=dst.total*dst.channels()  
        cv::Mat src(inFrame.height(),
                    inFrame.width(),
                    nType, inFrame.bits());
        cv::Mat dst(outFrame.height(),
                    outFrame.width(),
                    nType, outFrame.bits());
        cv::flip(src, dst, 1);  //最后一个参数flip_mode = 0 沿X-轴翻转, flip_mode > 0 (如 1) 沿Y-轴翻转， flip_mode < 0 (如 -1) 沿X-轴和Y-轴翻转  
        //dst = CTool::ImageRotate(src, cv::Point(inFrame.width() >> 1, inFrame.height() >> 1), m_pCamera->GetOrientation());  
        outFrame.unmap();
        emit sigCaptureFrame(outFrame);
#else
        //用QImage做图像镜像  
        QImage img(inFrame.bits(), inFrame.width(), inFrame.height(),
                   QVideoFrame::imageFormatFromPixelFormat(inFrame.pixelFormat()));
        QVideoFrame outFrame(img.mirrored(false, false));
        emit sigCaptureFrame(outFrame);
#endif
    }while(0);

    inFrame.unmap();
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

    do{
        if(-1 == nWidth)
            nWidth = inFrame.width();
        if(-1 == nHeight)
            nHeight = inFrame.height();

#if  RABBITIM_USE_FFMPEG && RABBITIM_USE_QXMPP
        //转换图片格式
        
        QVideoFrame outFrame;
        int nRet = 0;
        nRet = CTool::ConvertFormat(inFrame, outFrame, nWidth, nHeight, QVideoFrame::Format_YUYV);
        if(nRet)
        {
            LOG_MODEL_ERROR("CFrameProcess", "CFrameProcess::slotFrameConvertedToYUYV is fail");
            break;
        }
        if(outFrame.isValid())
            emit sigFrameConvertedToYUYVFrame(outFrame);
#else
    //#pragma message("Must use ffmpeg library and qxmpp library")
    #error "Must use ffmpeg library and qxmpp library"
#endif//#ifdef  RABBITIM_USE_FFMPEG && RABBITIM_USE_QXMPP
    }while(0);

    inFrame.unmap();
}

void CFrameProcess::slotFrameConvertedToRGB32(
        const QVideoFrame &frame, QRect rect)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess",
                    "CFrameProcess::slotFrameConvertedToRGB32 threadid:0x%X",
           QThread::currentThreadId());
#endif
    QVideoFrame outFrame;
    QVideoFrame inFrame(frame);

    if(inFrame.pixelFormat() == QVideoFrame::Format_RGB32
            && (rect.isEmpty()
                || (rect.width() == inFrame.width()
                    && rect.height() == inFrame.height())))
    {
        emit sigFrameConvertedToRGB32Frame(inFrame);
        return;
    }

    //QVideoFrame使用bits前一定要先map，bits才会生效  
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
        return;

    if(rect.isEmpty())
    {
        rect.setWidth(inFrame.width());
        rect.setHeight(inFrame.height());
    }

    do
    {
#ifdef RABBITIM_USE_FFMPEG
        //图片格式转换  
        int nRet = CTool::ConvertFormat(inFrame, outFrame, rect.width(), rect.height(),
                                        QVideoFrame::Format_RGB32);
        if(nRet)
        {
            LOG_MODEL_ERROR("CFrameProcess", "CFrameProcess::slotFrameConvertedToRGB32 faile");
            break;
        }
#else
    #pragma message("Must use ffmpeg or opencv library")
#endif
    }while(0);

    inFrame.unmap();

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
    int nRet = CTool::ConvertFormat(frame, outFrame, rect.width(),
                        rect.height(), QVideoFrame::Format_RGB32);
    if(nRet)
    {
        LOG_MODEL_WARNING("CFrameProcess", "CFrameProcess::slotFrameConvertedToRGB32 faile");
        return;
    }
#else
    #pragma message("Must use ffmpeg or opencv library")
#endif
    emit sigFrameConvertedToRGB32Frame(outFrame);
}
#endif
