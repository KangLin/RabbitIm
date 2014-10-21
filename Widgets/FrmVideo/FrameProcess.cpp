#include "FrameProcess.h"
#include "../../Tool.h"
#include "DataVideoBuffer.h"
#include "../../Global/Global.h"
#include "FrmVideo.h"
#include "Camera.h"

CFrameProcess::CFrameProcess(CCamera* pCamera, QObject *parent) :
    QObject(parent),
    m_pCamera(pCamera)
{
}

CFrameProcess::~CFrameProcess()
{
}

#ifdef ANDROID
//捕获视频帧。android下是图像格式是NV21,背景摄像头要顺时针旋转90度,再做Y轴镜像
//前景摄像头要逆时针旋转90度
void CFrameProcess::slotCaptureFrame(const QVideoFrame &frame)
{
    if(frame.pixelFormat() != QVideoFrame::Format_NV21)
    {
        LOG_MODEL_WARNING("Video", "CCaptureVideoFrame::present:don't Format_NV21");
        emit sigCaptureFrame(frame);
        slotFrameConvertedToYUYV(frame, 320,240);
        return;
    }

    QVideoFrame inFrame(frame);
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        LOG_MODEL_ERROR("Video", "CCaptureVideoFrame::present map error");
        return;
    }

    do{
        int nWidth = inFrame.width();
        int nHeight = inFrame.height();
        QByteArray mirror, rotate;
        CDataVideoBuffer *pBuffer = NULL;
        mirror.resize(inFrame.mappedBytes());
        rotate.resize(inFrame.mappedBytes());

        if(m_pCamera->GetCameraPoistion() == QCamera::BackFace)
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
    QVideoFrame inFrame(frame);
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        LOG_MODEL_ERROR("Video", "CCaptureVideoFrame::present map error");
        return;
    }

    do{
        //windows下要镜像，android下要旋转90度
        if(inFrame.pixelFormat() != QVideoFrame::Format_RGB32)
        {
            emit sigCaptureFrame(frame);
        }
        else
        {
#ifdef RABBITIM_USER_OPENCV
           //*用opencv库做图像镜像  
            QByteArray outData;
            outData.resize(inFrame.mappedBytes());//dst.total指图片像素个数，总字节数(dst.data)=dst.total*dst.channels()  
            cv::Mat src(inFrame.height(), inFrame.width(), CV_8UC4, inFrame.bits());
            cv::Mat dst(inFrame.height(), inFrame.width(), CV_8UC4, outData.data());
            cv::flip(src, dst, 0); //最后一个参数>0为x轴镜像，=0为y轴镜像，，<0为x,y轴都镜像。  
            //dst = CTool::ImageRotate(src, cv::Point(inFrame.width() >> 2, inFrame.height() >> 2), m_pCamera->GetOrientation());  

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
            QImage img(inFrame.bits(), inFrame.width(), inFrame.height(), QImage::Format_RGB32);
            img = img.mirrored(false, true);
            /*
            if(m_pCamera->GetOrientation())
                img = img.transformed(QTransform().rotate(m_pCamera->GetOrientation()));//*/
            QVideoFrame outFrame(img);
#endif
            emit sigCaptureFrame(outFrame);
        }

    }while(0);

    inFrame.unmap();

    return;
}
#endif

void CFrameProcess::slotFrameConvertedToYUYV(const QVideoFrame &frame, int nWidth, int nHeight)
{
    QVideoFrame inFrame(frame);
    if(!inFrame.map(QAbstractVideoBuffer::ReadOnly))
        return;

    do{
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
    }while(0);

    inFrame.unmap();
}

void CFrameProcess::slotFrameConvertedToRGB32(const QVideoFrame &inFrame, const QRect &rect)
{
    QVideoFrame outFrame;
    QVideoFrame f(inFrame);

    //QVideoFrame使用bits前一定要先map，bits才会生效
    if(!f.map(QAbstractVideoBuffer::ReadOnly))
        return;

    do
    {
        //图片格式转换
        AVPicture pic;
        int nRet = CTool::ConvertFormat(f, pic, rect.width(), rect.height(), AV_PIX_FMT_RGB32);
        if(nRet)
            break;

        FillFrame(pic, rect, outFrame);

        avpicture_free(&pic);
    }while(0);

    f.unmap();

    emit sigFrameConvertedToRGB32Frame(outFrame);
}

void CFrameProcess::slotFrameConvertedToRGB32(const QXmppVideoFrame &frame, const QRect &rect)
{
    //图片格式转换
    AVPicture pic;
    int nRet = CTool::ConvertFormat(frame, pic, rect.width(), rect.height(), AV_PIX_FMT_RGB32);
    if(nRet)
        return;

    QVideoFrame outFrame;
    FillFrame(pic, rect, outFrame);
    avpicture_free(&pic);
    emit sigFrameConvertedToRGB32Frame(outFrame);
}

int CFrameProcess::FillFrame(const AVPicture &pic, const QRect &rect, QVideoFrame &frame)
{
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
