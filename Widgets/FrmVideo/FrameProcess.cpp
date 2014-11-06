#include "FrameProcess.h"
#include "../../Tool.h"
#include "DataVideoBuffer.h"
#include "../../Global/Global.h"
#include "FrmVideo.h"
#include "Camera.h"
#include <QThread>

CFrameProcess::CFrameProcess(CCamera* pCamera, QObject *parent) :
    QObject(parent),
    m_pCamera(pCamera)
{
}

CFrameProcess::~CFrameProcess()
{
}

#ifdef ANDROID
//捕获视频帧。android下是图像格式是NV21,背景摄像头要顺时针旋转90度  
//前景摄像头要逆时针旋转90度  
void CFrameProcess::slotCaptureFrame(const QVideoFrame &frame)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess", "CFrameProcess::slotCaptureFrame threadid:0x%X",
           QThread::currentThreadId());
#endif
#ifdef DEBUG
    Q_ASSERT(m_pCamera);//需要初始指针  
#endif
    QVideoFrame inFrame(frame);
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        LOG_MODEL_ERROR("Video", "CCaptureVideoFrame::present map error");
        return;
    }

    do{

        if(frame.pixelFormat() == QVideoFrame::Format_RGB24)
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
            QImage img(inFrame.bits(), 
                       inFrame.width(), 
                       inFrame.height(),
                       QVideoFrame::imageFormatFromPixelFormat(inFrame.pixelFormat()));
            QMatrix m;
            img = img.transformed(m.rotate(360 - m_pCamera->GetOrientation()));

            QVideoFrame outFrame(img);
            emit sigCaptureFrame(outFrame);
            break;
        }

        if(frame.pixelFormat() != QVideoFrame::Format_NV21)
        {
            LOG_MODEL_WARNING("Video", "CCaptureVideoFrame::present:don't Format_NV21");
            QVideoFrame outFrame(frame);
            emit sigCaptureFrame(outFrame);
            break;
        }

        int nWidth = inFrame.width();
        int nHeight = inFrame.height();
        QByteArray mirror, rotate;
        CDataVideoBuffer *pBuffer = NULL;
        mirror.resize(inFrame.mappedBytes());
        rotate.resize(inFrame.mappedBytes());

        if(m_pCamera->GetCameraPoistion() == CCamera::BackFace)
        {
            //背景摄像头要顺时针旋转90度,再做Y轴镜像  
            CTool::YUV420spRotate90(reinterpret_cast<uchar *> (rotate.data()), inFrame.bits(), nWidth, nHeight, 1);
            CTool::YUV420spMirror(reinterpret_cast<uchar *> (mirror.data()),
                                  reinterpret_cast<uchar *>(rotate.data()),
                                  nHeight, nWidth, 0);
            pBuffer = new CDataVideoBuffer(mirror, nHeight, nWidth);
        }
        else
        {
            //前景摄像头要逆时针旋转90度  
            CTool::YUV420spRotate90(reinterpret_cast<uchar *> (rotate.data()), inFrame.bits(), nWidth, nHeight, -1);
            pBuffer = new CDataVideoBuffer(rotate, nHeight, nWidth);
        }

        QVideoFrame outFrame(pBuffer, QSize( nHeight, nWidth), QVideoFrame::Format_NV21);

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
    LOG_MODEL_DEBUG("CFrameProcess", "CFrameProcess::slotCaptureFrame threadid:0x%X",
           QThread::currentThreadId());
#endif
#ifdef DEBUG
    Q_ASSERT(m_pCamera);//需要初始指针  
#endif
    QVideoFrame inFrame(frame);
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        LOG_MODEL_ERROR("Video", "CCaptureVideoFrame::present map error");
        return;
    }

    do{
        //windows下要镜像
        /*if(inFrame.pixelFormat() != QVideoFrame::Format_RGB32)
        {
            emit sigCaptureFrame(frame);
            break;
        }*/

#ifdef RABBITIM_USER_OPENCV
        //*用opencv库做图像镜像  
        int nType = CV_8UC4;
        if(inFrame.pixelFormat() == QVideoFrame::Format_RGB24)
            nType = CV_8UC3;
        QByteArray outData;
        outData.resize(inFrame.mappedBytes());//dst.total指图片像素个数，总字节数(dst.data)=dst.total*dst.channels()  
        cv::Mat src(inFrame.height(), inFrame.width(), nType, inFrame.bits());
        cv::Mat dst(inFrame.height(), inFrame.width(), nType, outData.data());
        cv::flip(src, dst, 1);  //最后一个参数flip_mode = 0 沿X-轴翻转, flip_mode > 0 (如 1) 沿Y-轴翻转， flip_mode < 0 (如 -1) 沿X-轴和Y-轴翻转  
        //dst = CTool::ImageRotate(src, cv::Point(inFrame.width() >> 1, inFrame.height() >> 1), m_pCamera->GetOrientation());  
        
        //由QVideoFrame进行释放  
        CDataVideoBuffer* pBuffer = new CDataVideoBuffer(outData,
                                                         dst.cols,
                                                         dst.rows);
        QVideoFrame outFrame(pBuffer,
                             QSize(dst.cols,
                                   dst.rows),
                             inFrame.pixelFormat());//*/
#else
        //用QImage做图像镜像  
        QImage img(inFrame.bits(), inFrame.width(), inFrame.height(),
                   QVideoFrame::imageFormatFromPixelFormat(inFrame.pixelFormat()));
        img = img.mirrored(false, false);
        QVideoFrame outFrame(img);
#endif
        emit sigCaptureFrame(outFrame);
    }while(0);

    inFrame.unmap();

    return;
}
#endif

void CFrameProcess::slotFrameConvertedToYUYV(const QVideoFrame &frame, int nWidth, int nHeight)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess", "CFrameProcess::slotFrameConvertedToYUYV threadid:0x%X",
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
#ifdef  RABBITIM_USER_FFMPEG
        //转换图片格式
        AVPicture pic;
        int nRet = 0;
        nRet = CTool::ConvertFormat(inFrame, pic, nWidth, nHeight, AV_PIX_FMT_YUYV422);
        if(nRet)
        {
            LOG_MODEL_ERROR("Video", "CTool::ConvertFormat fail");
            break;
        }

        int size = avpicture_get_size(AV_PIX_FMT_YUYV422, nWidth, nHeight);
        QSize sizeFrame(nWidth, nHeight);
        QXmppVideoFrame f(size, sizeFrame, size / nHeight, QXmppVideoFrame::Format_YUYV);
        nRet = avpicture_layout(&pic, AV_PIX_FMT_YUYV422, nWidth, nHeight, f.bits(), size);
        if(nRet > 0)
        {
            emit sigFrameConvertedToYUYVFrame(f);
        }

        avpicture_free(&pic);
#else
#error "Must use ffmpeg library"
#endif//#ifdef  RABBITIM_USER_FFMPEG
    }while(0);

    inFrame.unmap();
}

void CFrameProcess::slotFrameConvertedToRGB32(const QVideoFrame &inFrame,  QRect rect)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess", "CFrameProcess::slotFrameConvertedToRGB32 threadid:0x%X",
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
#ifdef RABBITIM_USER_FFMPEG
        //图片格式转换
        AVPicture pic;
        LOG_MODEL_DEBUG("CFrmeProcess", "width:%d;height:%d;f.width:%d;height:%d", rect.width(), rect.height(), f.width(), f.height());
        int nRet = CTool::ConvertFormat(f, pic, rect.width(), rect.height(), AV_PIX_FMT_RGB32);
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

void CFrameProcess::slotFrameConvertedToRGB32(const QXmppVideoFrame &frame, QRect rect)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess", "CFrameProcess::slotFrameConvertedToRGB32 threadid:0x%X",
           QThread::currentThreadId());
#endif
    if(rect.isEmpty())
    {
        rect.setTopLeft(QPoint(0, 0));
        rect.setBottomRight(QPoint(frame.width(), frame.height()));
    }

    QVideoFrame outFrame;
#ifdef RABBITIM_USER_FFMPEG
    //图片格式转换  
    AVPicture pic;
    int nRet = CTool::ConvertFormat(frame, pic, rect.width(), rect.height(), AV_PIX_FMT_RGB32);
    if(nRet)
        return;
    FillFrame(pic, rect, outFrame);
    avpicture_free(&pic);
#else
#error "Must use ffmpeg or opencv library"
#endif
    emit sigFrameConvertedToRGB32Frame(outFrame);
}

#ifdef RABBITIM_USER_FFMPEG
int CFrameProcess::FillFrame(const AVPicture &pic, const QRect &rect, QVideoFrame &frame)
{
#ifdef DEBUG_VIDEO_TIME
    LOG_MODEL_DEBUG("CFrameProcess", "CFrameProcess::FillFrame threadid:0x%X",
           QThread::currentThreadId());
#endif
    int nRet = 0;
    int size = avpicture_get_size(AV_PIX_FMT_RGB32, rect.width(), rect.height());

    QByteArray outFrame;
    outFrame.resize(size);
    CDataVideoBuffer *pBuf = new CDataVideoBuffer(outFrame, rect.width(), rect.height());
    QVideoFrame f(pBuf, QSize(rect.width(), rect.height()), QVideoFrame::Format_RGB32);
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
#endif
